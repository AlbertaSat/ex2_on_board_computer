#include "demux_handler.h"


// void demux_init(DEMUX_Handler *handl, gioPORT_t *gio){
//     handl->gio_port_addr = gio;

//     gioInit();
// }

void set_pin(enum spi_cs_line_t cs_line, enum pin_state_t state) {
    switch(cs_line){
        case SPI_CS_N:
            //gioSetBit(demux_handler_t.gio_port_addr, SPI_CS_N, state);
            //SET OUTPUT Y7
        break;

        case SPI_CS_P:
            //SET OUTPUT Y13
        break;

        case SPI_CS_PD:
            //SET OUTPUT Y12
        break;

        case SPI_CS_S:
            //SET OUTPUT Y9
        break;

        case SPI_CS_SD:
            //SET OUTPUT Y10
        break;

        case SPI_CS_Z:
            //SET OUTPUT Y11
        break;

        default:
        break;
    };
}