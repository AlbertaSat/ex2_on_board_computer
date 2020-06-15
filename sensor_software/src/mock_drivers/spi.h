#ifndef SPI_H
#define SPI_H



void spi_write(uint32_t blocksize, uint16_t * srcbuff);

void spi_read(uint32_t blocksize, uint16_t * destbuff);

#endif