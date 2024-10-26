#include <xc.inc>

; LCD CONTROL PIN DEFINITIONS
LCD_RS	macro
        3            ; RS pin on RC3
	endm
LCD_RW  macro
	4            ; RW pin on RC4
	endm
LCD_EN  macro
	5            ; EN pin on RC5
	endm

; Variable declarations
    psect datas
temp_reg:
    DS 1          ; Temporary register for storing values
i:
    DS 1          ; Loop counter


    psect code
 ; LCD Command Write Subroutine
Lcd_CmdWrite:
    ; Send higher nibble of command
    movf    temp_reg, W       ; Load W with command
    andlw   0xF0              ; Mask lower 4 bits
    movwf   PORTD             ; Send higher nibble to PORTD
    bcf     PORTC, LCD_RS     ; RS = 0 (Command)
    bcf     PORTC, LCD_RW     ; RW = 0 (Write)
    bsf     PORTC, LCD_EN     ; Generate a High-to-low pulse on EN pin
    call    DelayUs           ; Short delay
    bcf     PORTC, LCD_EN     ; EN = 0
    call    DelayUs           ; Short delay

    ; Send lower nibble of command
    swapf   temp_reg, W       ; Swap nibbles in W
    andlw   0xF0              ; Mask lower nibble
    movwf   PORTD             ; Send lower nibble to PORTD
    bcf     PORTC, LCD_RS     ; RS = 0 (Command)
    bcf     PORTC, LCD_RW     ; RW = 0 (Write)
    bsf     PORTC, LCD_EN     ; EN = 1 (Enable pulse)
    call    DelayUs           ; Short delay
    bcf     PORTC, LCD_EN     ; EN = 0
    call    DelayUs           ; Short delay
    return

; LCD Data Write Subroutine
Lcd_DataWrite:
    ; Send higher nibble of data
    movf    temp_reg, W       ; Load W with data
    andlw   0xF0              ; Mask lower nibble
    movwf   PORTD             ; Send higher nibble to PORTD
    bsf     PORTC, LCD_RS     ; RS = 1 (Data)
    bcf     PORTC, LCD_RW     ; RW = 0 (Write)
    bsf     PORTC, LCD_EN     ; EN = 1 (Enable pulse)
    call    DelayUs           ; Short delay
    bcf     PORTC, LCD_EN     ; EN = 0
    call    DelayUs           ; Short delay

    ; Send lower nibble of data
    swapf   temp_reg, W       ; Swap nibbles in W
    andlw   0xF0              ; Mask lower nibble
    movwf   PORTD             ; Send lower nibble to PORTD
    bsf     PORTC, LCD_RS     ; RS = 1 (Data)
    bcf     PORTC, LCD_RW     ; RW = 0 (Write)
    bsf     PORTC, LCD_EN     ; EN = 1 (Enable pulse)
    call    DelayUs           ; Short delay
    bcf     PORTC, LCD_EN     ; EN = 0
    call    DelayUs           ; Short delay
    return

; Main program to initialize LCD and display messages
MAIN_LCD:
    bank1
    call    DelayMs           ; Short delay for LCD stabilization
    movlw   0x00
    movwf   TRISD             ; Configure PORTD as output
    movwf   TRISC             ; Configure PORTC as output
    bank0

    ; LCD Initialization in 4-bit mode
    movlw   0x02              ; Command for 4-bit initialization
    movwf   temp_reg
    call    Lcd_CmdWrite
    call    DelayMs

    movlw   0x28              ; Command for 2 lines, 5x7 mode
    movwf   temp_reg
    call    Lcd_CmdWrite
    call    DelayMs

    movlw   0x0E              ; Display ON, Cursor ON
    movwf   temp_reg
    call    Lcd_CmdWrite
    call    DelayMs

    movlw   0x01              ; Clear Display
    movwf   temp_reg
    call    Lcd_CmdWrite
    call    DelayMs

    movlw   0x80              ; Move cursor to first line
    movwf   temp_reg
    call    Lcd_CmdWrite
    call    DelayMs

    ; Write 'Hello world' on the LCD
    movlw   'H'
    movwf   temp_reg
    call    Lcd_DataWrite

    movlw   'e'
    movwf   temp_reg
    call    Lcd_DataWrite

    movlw   'l'
    movwf   temp_reg
    call    Lcd_DataWrite

    movlw   'l'
    movwf   temp_reg
    call    Lcd_DataWrite

    movlw   'o'
    movwf   temp_reg
    call    Lcd_DataWrite

    movlw   ' '
    movwf   temp_reg
    call    Lcd_DataWrite

    movlw   'w'
    movwf   temp_reg
    call    Lcd_DataWrite

    movlw   'o'
    movwf   temp_reg
    call    Lcd_DataWrite

    movlw   'r'
    movwf   temp_reg
    call    Lcd_DataWrite

    movlw   'l'
    movwf   temp_reg
    call    Lcd_DataWrite

    movlw   'd'
    movwf   temp_reg
    call    Lcd_DataWrite

    ; Enter infinite loop
    goto    MAIN_LCD

; Delay Subroutine for Microseconds
DelayUs:
    ; Adjust the number of iterations for the desired delay
    movlw   50                ; Load W with 50 iterations (50us approx.)
    movwf   0x22                 ; Store in i
DelayLoopUs:
    decfsz  0x22, F              ; Decrement i and skip if zero
    goto    DelayLoopUs       ; Loop again
    return

; Delay Subroutine for Milliseconds
DelayMs:
    ; Adjust the number of iterations for the desired delay
    movlw   250               ; Load W with 250 iterations (1 ms approx.)
    movwf   0x22                 ; Store in i
DelayLoopMs:
    decfsz  0x22, F              ; Decrement i and skip if zero
    goto    DelayLoopMs       ; Loop again
    return
