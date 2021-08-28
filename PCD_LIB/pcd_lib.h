/*
 * pcd_lib.h
 *
 *  Created on: August 27, 2021
 *  Author: Dawid Piwowarski
 */

#ifndef INC_PCD_LIB_H_
#define INC_PCD_LIB_H_

#include <stdio.h>


#define SPI_PORT spi0

#define LCD_MISO 16
#define LCD_MOSI 19
#define LCD_SCK 18
#define LCD_CS 17

#define LCD_RST 21
#define LCD_DC 20


#define PCD_WIDTH 84
#define PCD_HEIGHT 48

#define PD_POWERDOWN (1<<2)
#define V_VERTICAL (1<<1)
#define H_EXTENDED (1<<0)

#define BIAS0 0 // 1:100
#define BIAS1 0x01 // 1:80
#define BIAS2 0x02 // 1:65/1:65
#define BIAS3 0x03 // 1:48
#define BIAS4 0x04 // 1:40/1:34
#define BIAS5 0x05 // 1:24
#define BIAS6 0x06 // 1:18/1:16
#define BIAS7 0x07 // 1:10/1:9/1:8

#define TC0 0
#define TC1 0x01
#define TC2 0x02
#define TC3 0x03

#define DCONFIG_BLANK 0
#define DCONFIG_NORMAL 0x04
#define DCONFIG_ALLON 0x01
#define DCONFIG_INV 0x05

extern uint8_t * buffer;

int32_t PCD_init(void);

void PCD_SetCursor(uint8_t x, uint8_t y);
void PCD_Clear(void);

void PCD_CharWrite(char ch, uint8_t inv);
void PCD_StringWrite(char * str, uint8_t inv);
void PCD_int(int val, uint8_t radix, uint8_t inv);

void PCD_draw(void);

#endif /* INC_PCD_LIB_H_ */