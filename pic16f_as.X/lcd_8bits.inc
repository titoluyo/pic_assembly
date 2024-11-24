#include <xc.inc>
    
; pin definition
RS  macro
    PORTC, 3
    endm
RW  macro
    PORTC, 4
    endm
E   macro
    PORTC, 5
    endm
SEND	macro
        movwf   PORTD
	bsf	E
	call	Delay2
	bcf	E
	call	Delay2
	endm

    psect code
LCD_INIT:
    bank1
    clrf    TRISD
    clrf    TRISC
    bank0

    clrf    PORTD
    clrf    PORTC

    bcf	    RS
    bcf	    RW
    bcf	    E
    
    
    movlw   0x38
    SEND
    movlw   0x0F
    SEND
    movlw   0x01
    SEND
    
    bsf	    RS
    movlw   'H'
    SEND
    movlw   'O'
    SEND
    movlw   'L'
    SEND
    movlw   'A'
    SEND
    movlw   ' '
    SEND
    movlw   'M'
    SEND
    movlw   'U'
    SEND
    movlw   'N'
    SEND
    movlw   'D'
    SEND
    movlw   'O'
    SEND
    movlw   '*'
    SEND

LCD_LOOP:
    clrf    PORTD
    clrf    PORTC
    goto    LCD_LOOP
    



