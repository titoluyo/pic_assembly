#include "config.h"
#include "i2c.h"
#include "lcd_i2c.h"
#include "adc.h"
#include "uart.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 48000000
#endif

#define baudios 9600

char buffer[30]; // store string to send through serial port

unsigned int adc;
float voltaje = 0;
float lux = 0;
bool valido = 0;

void main(void) {
    ADC_Init();
    UART_Init();
    
    I2C_Init_Master(I2C_100KHZ);
    Lcd_Init();
    Lcd_Clear();
    __delay_ms(100);

    while (1) {
        valido = 0;
        adc = ADC_Read(0);
        voltaje = adc * 5.0f / 1023.0f;

        //AN0:
        //0-24
        if (voltaje >= 0 && voltaje < 1.91641) {
            lux = (voltaje * 11.811) - 0.3;
            valido = 1;
        }
        //25-49
        if (voltaje >= 1.91641 && voltaje < 2.64888) {
            lux = (voltaje * voltaje)*(13.798)-(voltaje * 29.234) + 30.366;
            valido = 1;
        }
        //50-74
        if (voltaje >= 2.64888 && voltaje < 3.07364) {
            lux = (voltaje * voltaje)*(29.775)-(voltaje * 111.88) + 137.46;
            valido = 1;
        }
        //75-109
        if (voltaje >= 3.07364 && voltaje < 3.4455) {
            lux = (voltaje * voltaje)*(55.857)-(voltaje * 270.93) + 380.08;
            valido = 1;
        }
        //110-159
        if (voltaje >= 3.4455 && voltaje < 3.76761) {
            lux = (voltaje * voltaje)*(116.54)-(voltaje * 686.62) + 1092.2;
            valido = 1;
        }
        //160-209
        if (voltaje >= 3.76761 && voltaje < 3.97135) {
            lux = (voltaje * voltaje)*(228.23)-(voltaje * 1522.1) + 2654.8;
            valido = 1;
        }
        //210-259
        if (voltaje >= 3.97135 && voltaje < 4.11306) {
            lux = (voltaje * voltaje)*(389.35)-(voltaje * 2796.0) + 5172.3;
            valido = 1;
        }
        //260-309
        if (voltaje >= 4.11306 && voltaje < 4.21789) {
            lux = (voltaje * voltaje)*(606.18)-(voltaje * 4574.3) + 8818.4;
            valido = 1;
        }
        //310-359
        if (voltaje >= 4.21789 && voltaje < 4.29888) {
            lux = (voltaje * voltaje)*(886.63)-(voltaje * 6935.0) + 13787.0;
            valido = 1;
        }
        //360-409
        if (voltaje >= 4.29888 && voltaje < 4.3635) {
            lux = (voltaje * voltaje)*(1233.3)-(voltaje * 9910.9) + 20174.0;
            valido = 1;
        }
        //410-459
        if (voltaje >= 4.3635 && voltaje < 4.41636) {
            lux = (voltaje * voltaje)*(1233.3)-(voltaje * 9910.9) + 20174.0;
            valido = 1;
        }

        //460-549
        if (voltaje >= 4.41636 && voltaje < 4.49087) {
            lux = (voltaje * voltaje)*(2615.4)-(voltaje * 22097.0) + 47038.0;
            valido = 1;
        }
        //550-599
        if (voltaje >= 4.49087 && voltaje < 4.52102) {
            lux = (voltaje * voltaje)*(9583.0)-(voltaje * 84751.0) + 187886.0;
            valido = 1;
        }
        //600-649
        if (voltaje >= 4.52102 && voltaje < 4.55279) {
            lux = (voltaje * voltaje)*(14671.0) - (voltaje * 131521.0) + 295338.0;
            valido = 1;
        }
        //650-699
        if (voltaje >= 4.55279 && voltaje < 4.57802) {
            lux = (voltaje * voltaje)*(4872.9) - (voltaje * 42513.0) + 93198.0;
            valido = 1;
        }
        //700-799
        if (voltaje >= 4.57802 && voltaje < 4.62024) {
            lux = (voltaje * voltaje)*(6454.2) - (voltaje * 57001.0) + 126383.0;
            valido = 1;
        }
        //800-1000
        if (voltaje >= 4.62024) {
            lux = (voltaje * voltaje)*(10127.0) - (voltaje * 90991.0) + 205023.0;
            valido = 1;
        }
        if (valido) {
            sprintf(buffer, "Lux: %.0f  ADC0 : %d  Voltaje: %.6f v6\r\n", lux, adc, voltaje);
            Uart_SString(buffer);
            sprintf(buffer, "Lux: %.0f  ", lux);
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_String(buffer);
            __delay_ms(100);
        }        
    }
    return;
}



