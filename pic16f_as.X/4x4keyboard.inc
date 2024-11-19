
    psect code
INICIO_KEYBOARD:
    bank1
    movlw	0x0F
    movwf	TRISB
    bank0
    movlw	0x0F
    movwf	PORTB
    bank1
    movlw	0x7F
    andwf	OPTION_REG, 1
    clrf	ADCON1

