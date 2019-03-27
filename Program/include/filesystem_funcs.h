
#ifndef FILESYSTEM_FUNCS_H
#define FILESYSTEM_FUNCS_H
#include <stdint.h>

typedef struct file {
    int fd;
    uint32_t total_size;

} File;

uint32_t get_file_size(unsigned char *source_file_name);

//allocates space for a file pointer
File *create_file(unsigned char *source_file_name);
void free_file(void *file);
int does_file_exist(unsigned char *source_file_name);
int get_offset(File *file, void *buff, uint32_t buf_size, int offset);
int write_offset(File *file, void *buff, uint32_t size, uint32_t offset);

#endif 
