#include <stdint.h>
#include "port.h"
#include "filesystem_funcs.h"
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include "utils.h"
#include "string.h"



uint32_t get_file_size(char *source_file_name) {

    int fd = ssp_open(source_file_name, 0);
    if (fd == -1){
        ssp_error("could not open file\n");
        return -1;
    }

    int bytes = ssp_lseek(fd, 0, 2);
    if (bytes == -1){
        ssp_error("could not seek file for file size\n");
        return -1;
    }

    ssp_lseek(fd, 0, 0);

    if (ssp_close(fd) == -1){
        ssp_error("could not close file\n");
        return -1;
    }

    return bytes;
}

File *create_file(char *source_file_name, int clear_file_contents) {

    int fd = 0;
    if (clear_file_contents){
        fd = ssp_open(source_file_name, O_RDWR | O_CREAT | O_TRUNC);
    }else {
        fd = ssp_open(source_file_name, O_RDWR | O_CREAT);
    }   
    if (fd == -1){
        ssp_error("couldn't create file\n");
        fd = ssp_open(source_file_name, O_RDWR);
        if (fd == -1) {
            ssp_error("count not open file\n");
            return NULL;
        }
    }


    uint32_t total_size = get_file_size(source_file_name);
    if (total_size == -1){
        ssp_error("couldn't get file size\n");
        return NULL;
    }

    File *file = ssp_alloc(1, sizeof(File));
    
    file->fd = fd;
    file->eof_checksum = 0;
    file->next_offset_to_send = 0;
    file->total_size = total_size;
    file->partial_checksum = 0;
    file->missing_offsets = linked_list();

    return file;

}


int does_file_exist(char *source_file_name) {

    int fd = ssp_open(source_file_name, O_RDWR);
    if (fd == -1){
        return 0;
    }
    if (ssp_close(fd) == -1){
        return -1;
    }

    return 1;
}

//modifys the seek location, returns bytes read
int get_offset(File *file, void *buff, uint32_t buf_size, int offset) {

    if (offset >= file->total_size){
        ssp_error("offset greater than file size\n");
        return -1;
    }

    if (ssp_lseek(file->fd, offset, SEEK_SET) == -1){
        ssp_error("could'nt set offset\n");
    }

    int bytes = ssp_read(file->fd, buff, buf_size);
    if (bytes == -1){
        ssp_error("Could not read anything from file\n");
    }

    return bytes;

}

int write_offset(File *file, void *buff, uint32_t size, uint32_t offset) {

    if (file == NULL) {
        ssp_error("Could not write, File struct is not created\n");
        return -1;
    }

    ssp_lseek(file->fd, (int) offset, SEEK_SET);
    int bytes = ssp_write(file->fd, buff, (size_t) size);

    if (bytes == -1){
        ssp_error("Could not write\n");
    }
    if (bytes < size && bytes >= 0){
        ssp_error("did not write all the bytes, this could be because the disk is full, or the file that was sent is empty!\n");
    }
    return bytes;
}

void free_file(void *file) {

    File *f = (File *) file;
    f->missing_offsets->free(f->missing_offsets, ssp_free);
    ssp_free(f);
}



uint32_t calc_check_sum(char *data, uint32_t length) {
    uint8_t remaining_bytes = length % 4;
    uint32_t check_sum = 0;
    uint32_t end = length - 4;
    for (unsigned int i = 0; i < end; i+= 4){
        check_sum += *((uint32_t *) &data[i]);
    }
    
    if (remaining_bytes){
        uint8_t last_chunk[4];
        memset(last_chunk, 0, 4);

        end = length - remaining_bytes;

        for (uint8_t i = 0; i < remaining_bytes; i++) {
            last_chunk[i] = data[end + i];;
        } 
          
        check_sum += *((uint32_t*) &last_chunk);        
    }

    return check_sum;
}


//stack buffer is the size of the packet length
uint32_t check_sum_file(File *file, uint16_t stack_buffer) {

    char buff[stack_buffer];
    uint32_t checksum = 0;
    uint32_t bytes_read = 0;
    for (int i = 0; i < file->total_size; i++) {
        
        bytes_read = get_offset(file, buff, stack_buffer, (int) stack_buffer);
        if (bytes_read > 0)
            checksum += calc_check_sum(buff, bytes_read);
    }

    return checksum;
}

static int find_nak(void *element, void* args) {

    Offset *offset_in_list = (Offset *) element;
    Offset *offset_to_insert = (Offset *) args;

    if (offset_to_insert->start >= offset_in_list->start && offset_to_insert->end <= offset_in_list->end) { 
        return 1;
    }

    return 0;
}

//ack is 1, nak is 0
int receive_offset(File *file, uint8_t ack, uint32_t offset_start, uint32_t offset_end) {
    
    List * nak_list = file->missing_offsets;

    Offset offset_to_insert;
    offset_to_insert.start = offset_start;
    offset_to_insert.end = offset_end;
    
    NODE * node = nak_list->findNode(nak_list, -1, find_nak, &offset_to_insert);
    if (node == NULL){
        ssp_printf("no begining node for receive_offset, or offset we already received, can't add new offset\n");
        return; 
    }

    Offset *offset_in_list = (Offset *) node->element;
    ssp_printf("received offset start:%u end:%u, found node: start:%u end:%u\n", offset_to_insert.start, offset_to_insert.end, offset_in_list->start, offset_in_list->end);

    if (offset_in_list->start == offset_in_list->end) {
        ssp_printf("removing node\n");  
        node->next->prev = node->prev;
        node->prev->next = node->next;
        ssp_free(node->element);
        ssp_free(node);
        nak_list->count--;
    }

    if (offset_to_insert.start > offset_to_insert.end)
        return;

    //insert new node
    if (offset_to_insert.start >= offset_in_list->start && offset_to_insert.end <= offset_in_list->end) {

        //remove node if both start and end are equal
        if (offset_to_insert.start == offset_in_list->start && offset_to_insert.end == offset_in_list->end) {
            ssp_printf("removing node\n");  
            node->next->prev = node->prev;
            node->prev->next = node->next;
            ssp_free(node->element);
            ssp_free(node);
            nak_list->count--;
            return;
        }

        //if new offset is in the start, simply change the list's node's start
        else if (offset_to_insert.start == offset_in_list->start && offset_to_insert.start < offset_in_list->end) {
            offset_in_list->start = offset_to_insert.end;
            return;
        }

        Offset *new_offset = ssp_alloc(1, sizeof(Offset));
        new_offset->start = offset_start;
        new_offset->end = offset_end;

        uint32_t tmp = new_offset->start;
        new_offset->start = offset_in_list->start;
        offset_in_list->start = new_offset->end;

        new_offset->end = tmp;

        NODE *cur = node;
        NODE *new = createNode(new_offset, new_offset->start);

        new->next = cur;
        new->prev = cur->prev;
        new->prev->next = new;
        cur->prev = new;
        nak_list->count++;
    }
}

File *create_temp_file(char *file_name) {
    File *file = create_file(file_name, 1);
    file->is_temp = 1;

    ssp_printf("mode acknowledged, building offset map\n");
    Offset *offset = ssp_alloc(1, sizeof(Offset));
    offset->end = TEMP_FILESIZE;
    offset->start = 0;
    file->missing_offsets->insert(file->missing_offsets, offset, TEMP_FILESIZE);
    return file;
}

static int print_nak(void *element, void* args) {

    Offset *offset_in_list = (Offset *) element;
    ssp_printf("start: %u, end: %u\n", offset_in_list->start, offset_in_list->end);

    return 0;
}


int change_tempfile_to_actual(char *temp, char *destination_file_name, uint32_t file_size, File *file) {

    //ssp_printf("renaming %s to: %s", temp, destination_file_name);
    ssp_rename(temp, destination_file_name);
    
    //file->missing_offsets->print(file->missing_offsets, print_nak, NULL);
    Offset* offset = (Offset*)file->missing_offsets->pop(file->missing_offsets);
    if (offset == NULL) {
        ssp_printf("no last node to pop\n");
        return -1;
    }


    offset->end = file_size;
    file->missing_offsets->push(file->missing_offsets, offset, file_size);
    file->is_temp = 0;
   
}
