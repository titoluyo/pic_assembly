#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic18f4550.h>
#include "configuration.h"
#include "semaforo.h"
#include "led7seg.h"

// TRISX - input:1, output:0
// LATX  - Escribir
// PORTX - Port

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

void main(void) {
    // SemaforoInit();
    // InterruptorInit();
    Led7segInit();
    
    
    while(1) {
        // InterruptorLoop();
        // SemaforoLoop();
        Led7segLoop();
    }    
    return;
}
