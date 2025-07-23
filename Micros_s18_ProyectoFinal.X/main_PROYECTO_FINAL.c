#include <xc.h>
#include "i2c.h"
#include "lcd_i2c.h"
#include "adc.h"
#include "uart.h"
#include <stdio.h>
#include <math.h>
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 48000000
#endif

char buffer[30];        // Buffer para armar mensajes a enviar
unsigned int valor;     // Valor del SPBRG calculado
unsigned int adc;       // Valor ADC AN0 (LDR)
float voltaje = 0;      // Valore de voltaje
float lux = 0;          // Valor de iluminancia estimado
char valido = 0;        // Flag para verificar si la lectura es valida

unsigned char pwm_duty; // Valor PWM (0-255)
float vLED = 0;         // Voltaje proporcional al PWM (teorico)

unsigned int adc2;      // Valor ADC AN1 (temperatura)
float vTEM = 0;         // Voltaje leido del sensor de temperatura
float temp = 0;         // Temperatura en grados Celsius

void main(void) {
    ADC_Init();             // Inicializa modulo ADC
    UART_Init();            // Inicializa modulo UART

    // Inicializaci�n del LCD por I2C
    I2C_Init_Master(I2C_100KHZ); // Inicia I2C a 100 kHz
    Lcd_Init();                 // Inicializa el LCD
    Lcd_Clear();                // Limpia la pantalla
    Lcd_Set_Cursor(0, 1);       // Cursor a fila 0 columna 1
    __delay_ms(100);           // Retardo inicial


    // Configuracion del PWM en RC2 (CCP1)
    TRISCbits.RC2 = 0;          // RC2 como salida
    PR2 = 0xFF;                 // Periodo maximo de 8 bits
    CCP1CON = 0b00001100;      // PWM modo en CCP1
    T2CON = 0b00000100;        // Timer2 ON con prescaler 1:1
    CCPR1L = 0;                // Ciclo util inicial 0%

    while (1) {
        // LECTURA DEL LDR EN AN0
        adc = ADC_Read(0); // Lee canal AN0
        voltaje = adc * 5.0f / 1023.0f; // Conversion a voltaje

        // PWM LED segun valor ADC
        pwm_duty = (unsigned char)(adc / 4);             // Ajuste de 10 bits a 8 bits
        CCPR1L = pwm_duty;             // Bits altos
        CCP1CONbits.DC1B = pwm_duty & 0x03; // Bits bajos
        vLED = pwm_duty * 5.0f / 253.0f; // Estimacion del voltaje/brillo a mostrar

        // SENSOR DE TEMPERATURA EN AN1
        adc2 = ADC_Read(1); // ADC del canal AN1
        vTEM = adc2 * 5.0f / 1023.0f; // Convierte el valor de ADC en Voltaje
        temp = vTEM * (150.0f + 55.f) / 5.0f - 55.0f; // Convierte el valor de voltaje en Temperatura

        // ENVIO POR SERIAL
        Uart_SString("-----------------------\r\n");
        sprintf(buffer, "ADC0 : %d   Voltaje: %.4f \r\n", adc, voltaje);
        Uart_SString(buffer);
  
        sprintf(buffer, "LED : %d    Vled: %.6f \r\n", pwm_duty, vLED);
        Uart_SString(buffer);

        sprintf(buffer, "Temp: %.0f C \r\n", temp);
        Uart_SString(buffer);

        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String(buffer); // Muestra temperatura
        __delay_ms(20);

        // CALCULO DE LUX SEGUN RANGO DE VOLTAJE
        if (voltaje >= 0 && voltaje < 1.91641) {
            lux = (voltaje * 11.811) - 0.3;
            valido = 1;
        } else if (voltaje >= 1.91641 && voltaje < 2.64888) {
            lux = (voltaje * voltaje)*(13.798)-(voltaje * 29.234) + 30.366;
            valido = 1;
        } else if (voltaje >= 2.64888 && voltaje < 3.07364) {
            lux = (voltaje * voltaje)*(29.775)-(voltaje * 111.88) + 137.46;
            valido = 1;
        } else if (voltaje >= 3.07364 && voltaje < 3.4455) {
            lux = (voltaje * voltaje)*(55.857)-(voltaje * 270.93) + 380.08;
            valido = 1;
        } else if (voltaje >= 3.4455 && voltaje < 3.76761) {
            lux = (voltaje * voltaje)*(116.54)-(voltaje * 686.62) + 1092.2;
            valido = 1;
        } else if (voltaje >= 3.76761 && voltaje < 3.97135) {
            lux = (voltaje * voltaje)*(228.23)-(voltaje * 1522.1) + 2654.8;
            valido = 1;
        } else if (voltaje >= 3.97135 && voltaje < 4.11306) {
            lux = (voltaje * voltaje)*(389.35)-(voltaje * 2796.0) + 5172.3;
            valido = 1;
        } else if (voltaje >= 4.11306 && voltaje < 4.21789) {
            lux = (voltaje * voltaje)*(606.18)-(voltaje * 4574.3) + 8818.4;
            valido = 1;
        } else if (voltaje >= 4.21789 && voltaje < 4.29888) {
            lux = (voltaje * voltaje)*(886.63)-(voltaje * 6935.0) + 13787.0;
            valido = 1;
        } else if (voltaje >= 4.29888 && voltaje < 4.3635) {
            lux = (voltaje * voltaje)*(1233.3)-(voltaje * 9910.9) + 20174.0;
            valido = 1;
        } else if (voltaje >= 4.3635 && voltaje < 4.41636) {
            lux = (voltaje * voltaje)*(1233.3)-(voltaje * 9910.9) + 20174.0;
            valido = 1;
        } else if (voltaje >= 4.41636 && voltaje < 4.49087) {
            lux = (voltaje * voltaje)*(2615.4)-(voltaje * 22097.0) + 47038.0;
            valido = 1;
        } else if (voltaje >= 4.49087 && voltaje < 4.52102) {
            lux = (voltaje * voltaje)*(9583.0)-(voltaje * 84751.0) + 187886.0;
            valido = 1;
        } else if (voltaje >= 4.52102 && voltaje < 4.55279) {
            lux = (voltaje * voltaje)*(14671.0) - (voltaje * 131521.0) + 295338.0;
            valido = 1;
        } else if (voltaje >= 4.55279 && voltaje < 4.57802) {
            lux = (voltaje * voltaje)*(4872.9) - (voltaje * 42513.0) + 93198.0;
            valido = 1;
        } else if (voltaje >= 4.57802 && voltaje < 4.62024) {
            lux = (voltaje * voltaje)*(6454.2) - (voltaje * 57001.0) + 126383.0;
            valido = 1;
        } else if (voltaje >= 4.62024) {
            lux = (voltaje * voltaje)*(10127.0) - (voltaje * 90991.0) + 205023.0;
            valido = 1;
        }

        // Muestra Lux por UART y LCD
        if (valido) {
            sprintf(buffer, "Lux: %.0f\r\n", lux);
            Uart_SString(buffer);
            Lcd_Set_Cursor(1, 2);
            Lcd_Write_String(buffer);
            __delay_ms(20);
        }
    }
    return;
}
