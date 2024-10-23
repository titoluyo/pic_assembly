psect datas
timer:           ; Variable for outer loop counter
    DS 1
temp:            ; Variable for shifting value
    DS 1
delay_count_outer: ; Outer delay loop variable
    DS 1
delay_count_inner: ; Inner delay loop variable
    DS 1

    psect code
MAIN_LED:
    call    Board_init;	Call initialization routine
    bank0
    movlw   0x00
    movwf   PORTB
    movlw   0x00
    movwf   PORTC
    
    bsf	    PORTC, 5; RC5=1

MAIN_LOOP_LED:
    ; Set temp to 0xFF at the start of each iteration
    movlw   0xFF
    movwf   temp

    ; Initialize timer to 0
    clrf    timer

FOR_LOOP_LED:
    ; Move 'temp' value to PORTD (Display temp on PORTD)
    movf    temp, W         ; Move 'temp' into W
    movwf   PORTD          ; Output W to PORTD (turn on LEDs)

    ; Delay for approximately
    call Delay2

    ; Right-shift 'temp' (temp >>= 1)
    rrf temp, F          ; Rotate Right 'temp' file register

    ; Increment 'timer'
    incf timer, F        ; Increment 'timer'

    ; Check if timer < 9
    movlw 10
    subwf timer, W       ; Subtract W (9) from 'timer'
    btfss STATUS, 2      ; Skip if timer equals 9
    goto FOR_LOOP_LED        ; If not 9, repeat the loop

    ; Repeat the main loop forever
    goto MAIN_LOOP_LED
