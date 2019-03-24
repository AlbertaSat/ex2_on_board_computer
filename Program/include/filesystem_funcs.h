
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


#endif 
