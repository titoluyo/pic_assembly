#include <xc.h>
#include <pic18f4550.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000  // 8Mhz 
#endif

// LED 7 segmento 
// Anodo comun   : send 0
// Cathodo comun : send 1

#define DISPLAY_1_DIR TRISBbits.TRISB0 // config TRIS disp1
#define DISPLAY_2_DIR TRISBbits.TRISB1 // config TRIS disp2
#define DISPLAY_3_DIR TRISBbits.TRISB2 // config TRIS disp3
#define DISPLAY_4_DIR TRISBbits.TRISB3 // config TRIS disp4
#define DISPLAY_1_PIN LATBbits.LATB0   // write on disp1
#define DISPLAY_2_PIN LATBbits.LATB1   // write on disp2
#define DISPLAY_3_PIN LATBbits.LATB2   // write on disp3
#define DISPLAY_4_PIN LATBbits.LATB3   // write on disp4

unsigned char display[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
//                          0     1     2     3     4     5     6     7     8     9

int unidades, decenas, centenas, millares, i;

void Mux2_init(void) {
    ADCON1bits.PCFG = 0x0F; // config all pins as digital
    DISPLAY_1_DIR = 0;  // DISP1 as output
    DISPLAY_2_DIR = 0;  // DISP2 as output
    DISPLAY_3_DIR = 0;  // DISP3 as output
    DISPLAY_4_DIR = 0;  // DISP4 as output
    TRISD = 0x00;  // All PORT D as output for 7 segment LED
    LATD = 0x00;   // turn off all leds of port for 7 segments LED
    DISPLAY_1_PIN = 0; // config as on of led 1 display
    DISPLAY_2_PIN = 0; // config as on of led 2 display
    DISPLAY_3_PIN = 0; // config as on of led 3 display
    DISPLAY_4_PIN = 0; // config as on of led 4 display
}

void Mux2_loop(void) {
    for (millares=0; millares<10; millares++) {
        for (centenas=0; centenas<10; centenas++) {
            for (decenas=0; decenas<10; decenas++) { // decenas incrementan cuando las unidades recorren
                for (unidades=0; unidades<10; unidades++) { // unidades van de 0 a 9 para subir 1 a decenas
                    // for (i=1;i<=10;i++) {  // se convierte en segundos
                        DISPLAY_1_PIN = 1;  // turn OFF DISP1
                        DISPLAY_2_PIN = 1;  // turn ON  DISP2
                        DISPLAY_3_PIN = 1;  // turn OFF DISP3
                        DISPLAY_4_PIN = 0;  // turn ON  DISP4
                        LATD = display[unidades];  // show number 3
                        __delay_ms(100);     // wait 30 ms

                        DISPLAY_1_PIN = 1;  // turn ON  DISP1
                        DISPLAY_2_PIN = 1;  // turn OFF DISP2
                        DISPLAY_3_PIN = 0;  // turn OFF DISP3
                        DISPLAY_4_PIN = 1;  // turn ON  DISP4
                        LATD = display[decenas];  // show number 4
                        __delay_ms(100);     // wait 30 ms

                        DISPLAY_1_PIN = 1;  // turn ON  DISP1
                        DISPLAY_2_PIN = 0;  // turn OFF DISP2
                        DISPLAY_3_PIN = 1;  // turn OFF DISP3
                        DISPLAY_4_PIN = 1;  // turn ON  DISP4
                        LATD = display[centenas];  // show number 4
                        __delay_ms(100);     // wait 30 ms

                        DISPLAY_1_PIN = 0;  // turn ON  DISP1
                        DISPLAY_2_PIN = 1;  // turn OFF DISP2
                        DISPLAY_3_PIN = 1;  // turn OFF DISP3
                        DISPLAY_4_PIN = 1;  // turn ON  DISP4
                        LATD = display[millares];  // show number 4
                        __delay_ms(100);     // wait 30 ms

                    // }
                }
            }
        }
    }
    
}
