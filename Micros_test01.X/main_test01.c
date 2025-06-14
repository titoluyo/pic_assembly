#include <xc.h>
#include "config.h"

#define _XTAL_FREQ 4000000

void main(void) {
    ADCON1 = 0x0F; // todos los pines a digitales
    TRISDbits.TRISD0 = 0;  // RD0 como salida
    LATDbits.LATD0 = 1;
    TRISDbits.TRISD1 = 0;  // RD0 como salida
    LATDbits.LATD1 = 1;
    return;
}
