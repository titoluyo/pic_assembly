#include <xc.inc>
    
    psect code
STEPPER:
    bank1
    clrf    TRISA
    clrf    TRISC
    clrf    TRISD
    bank0
    clrf    PORTD	    ; clean up PORTD
    bcf	    PORTA, 3	    ; RA3=0 ; 
    bcf	    PORTC, 3	    ; RC3=0 ; disable leds
    call    Delay1
    bcf	    PORTC, 3	    ; RC3=0 ; disable leds
    bsf	    PORTC, 4	    ; RC4=1 ; enable ULN2003
    bcf	    PORTC, 5	    ; RC5=0 ; disable leds
LOOP_STEPPER:
    ;movlw   00000111B	    ; 0x07
    movlw   00000011B
    movwf   PORTD
    call    Delay1
    
    ;movlw   00001011B	    ; 0x0B
    movlw   00000110B
    movwf   PORTD
    call    Delay1

    ;movlw   00001101B	    ; 0x0D
    movlw   00001100B
    movwf   PORTD
    call    Delay1

    ;movlw   00001110B	    ; 0x0E
    movlw   00001001B
    movwf   PORTD
    call    Delay1

    goto    LOOP_STEPPER
