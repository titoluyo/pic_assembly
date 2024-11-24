;macro definition
stepper macro arg1
    movlw   arg1
    movwf   PORTD
    call    Delay2
    endm
    
    psect code
STEPPER:
    bank1
    clrf    TRISA	    ; PORTA output
    clrf    TRISC	    ; PORTC output
    clrf    TRISD	    ; PORTD output
    bank0
    clrf    PORTD	    ; clean up PORTD
    bcf	    PORTA, 3	    ; RA3=0 ; 
    bcf	    PORTC, 3	    ; RC3=0 ; disable leds
    call    Delay1
    bcf	    PORTC, 3	    ; RC3=0 ; disable leds
    bsf	    PORTC, 4	    ; RC4=1 ; enable ULN2003
    bsf	    PORTC, 5	    ; RC5=0 ; disable leds
LOOP_STEPPER:
    stepper 00000001B
    stepper 00000011B
    stepper 00000010B
    stepper 00000110B
    stepper 00000100B
    stepper 00001100B
    stepper 00001000B
    stepper 00001001B
    goto    LOOP_STEPPER
