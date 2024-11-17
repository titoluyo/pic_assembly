#include <xc.inc>


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
	call	Delay
	bcf	E
	call	Delay
	endm


    psect datas
delay_count_outer: ; Outer delay loop variable
    DS 1
delay_count_inner: ; Inner delay loop variable
    DS 1
    
    psect program, class = CODE, reloc=2
PROGRAM:
    goto    START
    
    psect code
START:
    bank1
    clrf    TRISD
    clrf    TRISC
;    bcf	    TRISD, 0	; set RD0 as output
    bank0
    clrf    PORTD
    clrf    PORTC

    bcf	    RS	    ; RS = 0 ; instruction register
    bcf	    RW	    ; RW = 0 ; write mode
    bcf	    PORTC, 5 ;E
    
LCD:
    movlw   0x38    ; send SET command for 8 bit mode by 2 lines
    SEND
    movlw   0x0F    ; Turns on the display, enables the cursor, and sets the cursor to blink.
    SEND
    movlw   0x01    ; Clears the display
    SEND
   
    bsf	    RS	    ; RS = 1; data register
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

    clrf    PORTD
    clrf    PORTC

    
LOOP:
    bsf	    PORTD, 0	; RD0 on
    call    Delay
    bcf	    PORTD, 0    ; RD0 off
    call    Delay
    goto    LOOP	; never ending loop

;-----------------
; Delay Subroutine 
Delay:
    ; Outer loop counter (repeat for a long delay)
    movlw   0x10  ; 20ms
    movwf   delay_count_inner            ; Outer loop counter in memory location 0x22
    goto    DelayOuterLoop

DelayOuterLoop:
    movlw   0xCF
    movwf   delay_count_outer ;0x23	    ; Inner loop counter in memory location 0x23

DelayInnerLoop:
    decfsz  delay_count_outer, F ;0x23, F	    ; Decrement inner loop counter, skip if zero
    goto    DelayInnerLoop  ; Loop until 0x23 becomes 0

    decfsz  delay_count_inner, F ;0x22, F	    ; Decrement outer loop counter, skip if zero
    goto    DelayOuterLoop  ; Loop until 0x22 becomes 0

    return              ; Return from delay subroutine

    end
