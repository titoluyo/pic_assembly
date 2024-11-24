#define SWT	    PORTB, 0
#define LED	    PORTD, 0
#define SWT_TRIS   TRISB, 0
#define LED_TRIS    TRISD, 0
    
    psect code
INICIO_PULSADOR:
    bcf	    LED
    bcf	    STATUS, 6	; RP1=0
    bsf	    STATUS, 5	; RP0=1 ; 01 Bank1
    bcf	    LED_TRIS	; output
    bsf	    SWT_TRIS	; input
    bcf	    TRISA, 3	; RA3 output
    bcf	    TRISC, 5	; RC5 output
    bcf	    STATUS, 5	; RP0=0	; 00 Bank0
    
    bcf	    PORTA, 3	; RA3=0 (!OE)
    bsf	    PORTC, 5	; RC5=1 (enable LEDs latch)
START_PULSADOR:
    btfsc   SWT		; SWITCH == 0 ? -> 1
    goto    APAGAR
    bsf	    LED
    goto    START_PULSADOR
APAGAR:
    bcf	    LED
    goto    START_PULSADOR
