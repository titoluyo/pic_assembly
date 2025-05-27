#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic18f4550.h>
#include "configuration.h"
//#include "semaforo.h"
//#include "led7seg.h"
//#include "mux2.h"
#include "lcd16x2.h"

// TRISX - input:1, output:0
// LATX  - Escribir
// PORTX - Port

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

void main(void) {
    // SemaforoInit();
    // InterruptorInit();
    // Led7segInit();
    // Mux2_init();
    Lcd16x2_init();
    
    
    while(1) {
        // InterruptorLoop();
        // SemaforoLoop();
        // Led7segLoop();
        // Mux2_loop();
        Lcd16x2_loop();
    }    
    return;
}
