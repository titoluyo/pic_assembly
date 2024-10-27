#include <xc.inc>
    
    psect code
STEPPER:
    bank1
    clrf    TRISA
    clrf    TRISC
    clrf    TRISD
    bank0
    bcf	    PORTA, 3	    ; RA3=0 ; 
    bsf	    PORTC, 4	    ; RC4=1 ; enable ULN2003
    bcf	    PORTC, 5	    ; RC5=0 ; disable leds
LOOP_STEPPER:
    movlw   0xF7
    movwf   PORTD
    call    Delay1
    
    movlw   0xFB
    movwf   PORTD
    call    Delay1

    movlw   0xFD
    movwf   PORTD
    call    Delay1

    movlw   0xFE
    movwf   PORTD
    call    Delay1

    goto    LOOP_STEPPER
