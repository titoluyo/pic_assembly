#include "config.h"         // Configuracion de los bits del microcontrolador
#include "i2c.h"            // Funciones para comunicacion I2C
#include "lcd_i2c.h"        // Controlador de pantalla LCD por I2C
#include "adc.h"            // Funciones para ADC (conversion analogica-digital)
#include "uart.h"           // Funciones para comunicacion serial UART
#include <stdbool.h>        // Libreria para usar tipo de dato booleano
#include <stdio.h>          // Para usar sprintf()
#include <math.h>           // Para funciones matematicas

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 48000000 // Frecuencia del oscilador
#endif

char buffer[30];            // Cadena de texto para enviar por UART

unsigned int adc;           // Valor leido del ADC
float voltaje = 0;          // Valor convertido a voltaje
float lux = 0;              // Valor convertido a lux
bool valido = 0;            // Bandera para indicar si la lectura es valida

void main(void) {
    ADC_Init();             // Inicializa modulo ADC
    UART_Init();            // Inicializa modulo UART
    
    I2C_Init_Master(I2C_100KHZ); // Inicializa I2C como maestro a 100 kHz
    Lcd_Init();             // Inicializa pantalla LCD
    Lcd_Clear();            // Limpia pantalla
    __delay_ms(100);        // Espera para estabilizar

    while (1) {             // Bucle principal infinito
        valido = 0;         // Reinicia la bandera de validacion
        adc = ADC_Read(0);  // Lee canal AN0 (ADC)
        voltaje = adc * 5.0f / 1023.0f; // Convierte ADC a voltaje (0-5V)

        // A partir del voltaje, se selecciona un rango para aplicar formula de conversion a lux
        // Cada rango aplica una ecuacion cuadratica ajustada experimentalmente (Excel)

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
        
        // Si la lectura fue valida, se envian y muestran los resultados
        if (valido) {
            sprintf(buffer, "Lux: %.0f  ADC0 : %d  Voltaje: %.6f v6\r\n", lux, adc, voltaje);
            Uart_SString(buffer);         // Envia resultado por UART
            sprintf(buffer, "Lux: %.0f  ", lux);
            Lcd_Set_Cursor(1, 1);         // Mueve cursor a linea 1, columna 1
            Lcd_Write_String(buffer);     // Muestra el valor de lux en la pantalla
            __delay_ms(100);              // Espera antes de la siguiente lectura
        }        
    }
    return;
}



