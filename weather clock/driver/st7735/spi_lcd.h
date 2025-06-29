#ifndef _SPI_LCD_H
#define _SPI_LCD_H

void spi_cs(uint8_t BitVal);
void spi_send(uint8_t data);
void myspi_init(void);
void spi_sda(uint8_t BitVal);
void spi_scl(uint8_t BitVal);

#endif
