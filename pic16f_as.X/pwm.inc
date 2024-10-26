#include <xc.inc>
    
    psect code
PWM_INIT:
    bank1
    movlw   77
    movwf   PR2
    bcf	    TRISD, 0
    bcf	    TRISC, 2	; RC2
    bank0
    movlw   00111110B	; 80% MSB
    movwf   CCPR1L
    bsf	    CCP1CON, 5	; 1 LSB 80%
    bcf	    CCP1CON, 4  ; 0
    bsf	    T2CON, 1	; 16 Prescaler
    bcf	    T2CON, 0
    bsf	    T2CON, 2	; TMR2 ON
    bsf	    CCP1CON, 3	; 11XX PWM
    bsf	    CCP1CON, 2
    bsf	    CCP1CON, 1
    bsf	    CCP1CON, 0
START_PWM:
    bsf	    PORTD, 0
    goto    START_PWM


