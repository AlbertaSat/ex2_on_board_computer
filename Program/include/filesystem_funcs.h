
#ifndef FILESYSTEM_FUNCS_H
#define FILESYSTEM_FUNCS_H
#include <stdint.h>
#include "utils.h"

#define TEMP_FILESIZE 20000

typedef struct offset {
    uint32_t start;
    uint32_t end;
} Offset;


typedef struct file {
    int fd;
    uint32_t next_offset_to_send;
    uint32_t total_size;
    uint32_t partial_checksum;
    uint32_t eof_checksum;
    List *missing_offsets;

} File;

uint32_t get_file_size(char *source_file_name);

//allocates space for a file pointer
File *create_file(char *source_file_name, int clear_file_contents);
void free_file(void *file);
int does_file_exist(char *source_file_name);
int get_offset(File *file, void *buff, uint32_t buf_size, int offset);
int write_offset(File *file, void *buff, uint32_t size, uint32_t offset);
uint32_t calc_check_sum(char *data, uint32_t length);
uint32_t check_sum_file(File *file, uint16_t stack_buffer);
void receive_offset(File *file, uint8_t ack, uint32_t offset_start, uint32_t offset_end);
File *create_temp_file(uint64_t transaction_sequence_num);
#endif 
