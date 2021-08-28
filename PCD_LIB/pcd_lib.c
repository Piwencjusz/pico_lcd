/*
 * pcd_lib.c
 *
 *  Created on: August 27, 2021
 *  Author: Dawid Piwowarski
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "pcd_lib.h"
#include "ascii.h"

uint8_t * buffer;

static int32_t PCD_SendCommand(uint8_t cmd)
{
    gpio_put(LCD_DC, 0);
    gpio_put(LCD_CS, 0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
	gpio_put(LCD_CS, 1);

	return 0;
}

static int32_t PCD_SendData(uint8_t data)
{
    gpio_put(LCD_DC, 1);
    gpio_put(LCD_CS, 0);
	spi_write_blocking(SPI_PORT, &data, 1);
	gpio_put(LCD_CS, 1);

	return 0;
}

int32_t PCD_init(void)
{
	if ((!buffer) && !(buffer = (uint8_t *)malloc(504)))
	{
	    return 0;
	}

	memset(buffer, 0, 504);

    spi_init(SPI_PORT, 100000);

    gpio_set_function(LCD_MISO, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(LCD_SCK, GPIO_FUNC_SPI);
    
    gpio_init(LCD_CS);
    gpio_init(LCD_RST);
    gpio_init(LCD_DC);

    gpio_set_dir(LCD_CS, GPIO_OUT);
    gpio_set_dir(LCD_RST, GPIO_OUT);
    gpio_set_dir(LCD_DC, GPIO_OUT);

    gpio_put(LCD_CS, 1);
    gpio_put(LCD_RST, 1);
    gpio_put(LCD_DC, 1);

	gpio_put(LCD_RST, 0);
	sleep_ms(10);
	gpio_put(LCD_RST, 1);

	PCD_SendCommand(0x20 | (H_EXTENDED & 0x07));
	PCD_SendCommand(0x10 | (BIAS4 & 0x07));
	PCD_SendCommand(0x80 | (60 & 0x7f));
	PCD_SendCommand(0x04 | (TC0 & 0x03));

	PCD_SendCommand(0x20 | (0 & 0x07));
	PCD_SendCommand(0x08 | (DCONFIG_NORMAL & 0x05));

	PCD_Clear();

	return 1;
}

void PCD_SetCursor(uint8_t x, uint8_t y)
{
	PCD_SendCommand(0x40 | ( y & 0x07 ));
	PCD_SendCommand(0x80 | ( x & 0x7f ));
}

void PCD_Clear(void)
{
    uint8_t x= 0;

    gpio_put(LCD_DC, 1);
    gpio_put(LCD_CS, 0);
	for( uint16_t i= 0; i<504; i++ ) {
        spi_write_blocking(SPI_PORT, &x, 1);
	}
	gpio_put(LCD_CS, 1);

	PCD_SetCursor(0, 0);
}

void PCD_draw(void)
{
	uint8_t x= 0;

	PCD_SetCursor(0, 0);

	gpio_put(LCD_DC, 1);
    gpio_put(LCD_CS, 0);
    	for( uint16_t i= 0; i<504; i++ ) {
			x= buffer[i];
        spi_write_blocking(SPI_PORT, &x, 1);
	}
	gpio_put(LCD_CS, 1);
}

void PCD_CharWrite(char ch, uint8_t inv)
{
	for (int i = 0; i < 5; i++) {
		if(!inv) PCD_SendData(ascii[(ch-32)][i]);
		else PCD_SendData(~ascii[(ch-32)][i]);
	}
	if(!inv) PCD_SendData(0);
	else PCD_SendData(0xff);
}

void PCD_StringWrite(char * str, uint8_t inv)
{
  while (*str) PCD_CharWrite(*str++, inv);
}

void PCD_int(int val, uint8_t radix, uint8_t inv)
{
	char bufor[17];
	PCD_StringWrite( itoa(val, bufor, radix ), inv);
}