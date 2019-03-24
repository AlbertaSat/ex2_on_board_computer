#include <stdint.h>
#include <port.h>
#include <fcntl.h>
#include <stddef.h>


uint32_t get_file_size(unsigned char *source_file_name) {

    int fd = ssp_open(source_file_name, 0);
    if (fd == -1){
        return -1;
    }

    int bytes = ssp_lseek(fd, 0, 2);
    if (bytes == -1){
        return -1;
    }

    ssp_lseek(fd, 0, 0);

    if (ssp_close(fd) == -1){
        return -1;
    }

    return bytes;
}


File *create_file(unsigned char *source_file_name) {

    int fd = ssp_open(source_file_name, O_RDWR | O_CREAT);
    if (fd == -1){
        ssp_error("couldn't open file\n");
        return NULL;
    }

    uint32_t total_size = get_file_size(source_file_name);
    if (total_size == -1){
        ssp_error("couldn't get file size\n");
        return NULL;
    }

    File *file = ssp_alloc(1, sizeof(File));
    file->fd = fd;
    file->total_size = total_size;

    return file;

}

void free_file(void *file) {
    ssp_free(file);
}