#ifndef DEMUX_HANDLER_H
#define DEMUX_HANDLER_H

//#include "gio.h"

enum pin_state_t{
    LOW = 0,
    HIGH
};

enum spi_cs_line_t{
    SPI_CS_P = 0,
    SPI_CS_PD,
    SPI_CS_Z,
    SPI_CS_SD,
    SPI_CS_S,
    SPI_CS_N 
};

typedef struct demux_handler_t DEMUX_Handler;

struct demux_handler_t{
    //gioPORT_t *gio_port_addr;
}demux_handler_t;


// void demux_init(DEMUX_Handler *handl, gioPORT_t *gio);

void set_pin(enum spi_cs_line_t, enum pin_state_t);

#endif // DEMUX_HANDLER_H
