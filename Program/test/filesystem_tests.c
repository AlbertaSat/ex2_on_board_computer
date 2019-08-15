#include "filesystem_funcs.h"
#include "filesystem_tests.h"



static void nak_print(void *element, void *args){
    Offset *offset = (Offset *)element;
    ssp_printf("start: %u end: %u\n", offset->start, offset->end);
}



static receive_offset_tests(){

    File *file = create_temp_file("temp_test");

    receive_offset(file, 0, 5, 50);
    
    file->missing_offsets->iterate(file->missing_offsets, nak_print, 0);
    receive_offset(file, 0, 100, 1000);

    file->missing_offsets->iterate(file->missing_offsets, nak_print, 0);
    receive_offset(file, 0, 50, 100);
    file->missing_offsets->iterate(file->missing_offsets, nak_print, 0);

}

int file_system_tests() {
    int error = 0;
    error = receive_offset_tests();
    return error;
}