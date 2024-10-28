psect datas
timer:           ; Variable for outer loop counter
    DS 1
temp:            ; Variable for shifting value
    DS 1
delay_count_outer: ; Outer delay loop variable
    DS 1
inner: ; Inner delay loop variable
    DS 1
GUARDAR_DATO:
    DS 1
COMANDO:
    DS 1
DATA_GUARDAR:
    DS 1
DATAS:
    DS 1
CUENTA:
    DS 1
DATO:
    DS 1
    
;----------------------------------------
; macros
;----------------------------------------
bank0 macro
    bcf	    STATUS, 6	    ; RP1=0
    bcf	    STATUS, 5	    ; RP0=0
    endm

bank1 macro
    bcf	    STATUS, 6	    ; RP1=0
    bsf	    STATUS, 5	    ; RP0=1
    endm

bank2 macro
    bsf	    STATUS, 6	    ; RP1=1
    bcf	    STATUS, 5	    ; RP0=0
    endm

bank3 macro
    bsf	    STATUS, 6	    ; RP1=1
    bsf	    STATUS, 5	    ; RP0=1
    endm

input macro arg1, arg2
    bank1
    bsf	    arg1, arg2
    bank0
    endm

output macro arg1, arg2
    bank1
    bcf	    arg1, arg2
    bank0
    endm
    
;----------------------------------------
; Initialization routine
;----------------------------------------
    psect code
Board_init:
    ; Select Bank 1 to configure TRIS registers
    bank1
    ; Set all TRIS registers to 0 (set all pins as outputs)
    clrf    TRISA           ; Set PORTA as output
    clrf    TRISB           ; Set PORTB as output
    clrf    TRISC           ; Set PORTC as output
    clrf    TRISD           ; Set PORTD as output
    clrf    TRISE           ; Set PORTE as output
    
    bank0
    // set all 8 PORTD pins to 1 
    movlw   0xFF
    movwf   PORTD
    
    // Set "latch output enable" ON for all three 74HC573 latches
    bcf	    PORTA, 3	    ; RA3=0
    
    // Latch 1
    bsf	    PORTC, 5
    nop
    nop
    bcf	    PORTC, 5

    // Latch 2
    bsf	    PORTC, 3
    nop
    nop
    bcf	    PORTC, 3

    // Latch 3
    movlw   0x00	    ; set all 8 PORTD pins to 0
    movwf   PORTD
    bsf	    PORTC, 4
    nop
    nop
    bcf	    PORTC, 4

    // Latch 3
    movlw   0xFF	    ; set all 8 PORTD pins to 1
    movwf   PORTD
    bsf	    PORTC, 2
    bcf	    PORTE, 0
    bcf	    PORTE, 1
    bcf	    PORTE, 2

    return

;-----------------
    ; Delay Subroutine 
Delay2:
    ; Outer loop counter (repeat for a long delay)
    movlw   0x60
    movwf   0x22           ; Outer loop counter in memory location 0x22
    goto    DelayOuterLoop

Delay1:
    ; Outer loop counter (repeat for a long delay)
    movlw   0x10
    movwf   0x22           ; Outer loop counter in memory location 0x22
    goto    DelayOuterLoop

DelayOuterLoop:
    ; Inner loop for fine-tuning delay
    movlw   0xCF
    movwf   0x23           ; Inner loop counter in memory location 0x23

DelayInnerLoop:
    decfsz  0x23, F       ; Decrement inner loop counter, skip if zero
    goto    DelayInnerLoop  ; Loop until 0x23 becomes 0

    decfsz  0x22, F       ; Decrement outer loop counter, skip if zero
    goto    DelayOuterLoop  ; Loop until 0x22 becomes 0

    return               ; Return from delay subroutine
