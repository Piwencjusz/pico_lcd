/*
 *  main.c
 *
 *  Created on: August 27, 2021
 *  Author: Dawid Piwowarski
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/adc.h"

#include "PCD_LIB/pcd_lib.h"


#define LED 25


uint16_t cnt= 0;

int main()
{
    stdio_init_all();

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, 1);

    PCD_init();

    PCD_StringWrite("Hello world!", 0);
    PCD_SetCursor(0, 1);
    PCD_StringWrite("RP Pico", 0);
    PCD_SetCursor(0, 2);
    PCD_StringWrite("RP2040", 0);

    while(1)
    {
        uint16_t raw= adc_read();
        const float conv= 3.3f/(1<<12);
        float voltage= raw * conv;
        float temp= 27 - (voltage - 0.706)/0.001721;

        char c[16];
        sprintf(c, "T: %g*C", temp);

        PCD_SetCursor(0, 4);
        PCD_StringWrite(c, 0);

        PCD_SetCursor(0, 5);
        PCD_int(cnt++, 10, 0);

        gpio_put(LED, 1);
        sleep_ms(500);
        gpio_put(LED, 0);
        sleep_ms(500);
    }
}