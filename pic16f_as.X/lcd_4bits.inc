; macros
RS  macro
    PORTC, 0
    endm
    
E   macro
    PORTC, 1
    endm

;RW  macro
;    PORTC, 2
;    endm
    

LCD_INIT:
    bsf	    STATUS, 5
    clrf    TRISD; output
    clrf    TRISC; output
    bcf	    STATUS, 5
    
    return
