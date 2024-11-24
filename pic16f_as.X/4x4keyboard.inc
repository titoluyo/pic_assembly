    psect datas
crap:
    DS	1
key:
    DS	1
    
    
    psect code
INICIO_KEYBOARD:
    ;TRISB       = 0x0F;  
    movlw	0x0F
    bank1
    movwf	TRISB
    
    ;PORTB       = 0x0F; 
    movlw	0xF0
    movlw	0x0F
    bank0
    movwf	PORTB
    
    ;OPTION_REG &= 0x7F;     
    MOVLW 0x7F
    BSF STATUS, 0x5
    BCF STATUS, 0x6
    ANDWF OPTION_REG, F
    ;movlw	0x7F
    ;bank1
    ;andwf	OPTION_REG, 1
    
    ;ADCON1      = 0x00;
    clrf	ADCON1
LOOP_KEYB:
    nop
    
    ;key = PORTB & 0x0F;            
    BCF STATUS, 0x5
    BCF STATUS, 0x6
    MOVF PORTB, W
    MOVWF 0x0C88 ;key
    MOVLW 0xF
    ANDWF key, F
    
    ;if(key != 0x0F)   
    MOVLW 0xF
    XORWF 0x0C88, W ;key, W
    BTFSC STATUS, 0x2
    GOTO LOOP_KEYB1
    GOTO LOOP_KEYB2
LOOP_KEYB1:
    nop
LOOP_KEYB2:
    nop
    
    goto	LOOP_KEYB

