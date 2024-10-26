#include <xc.inc>
#include "common.inc"
#include "led.inc"
#include "lcd_8bits.inc"

; CONFIG
  CONFIG  FOSC = XT             ; Oscillator Selection bits (HS oscillator)
  CONFIG  WDTE = OFF            ; Watchdog Timer Enable bit (WDT disabled)
  CONFIG  PWRTE = ON            ; Power-up Timer Enable bit (PWRT enabled)
  CONFIG  BOREN = OFF           ; Brown-out Reset Enable bit (BOR disabled)
  CONFIG  LVP = OFF             ; Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
  CONFIG  CPD = OFF             ; Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
  CONFIG  WRT = OFF             ; Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
  CONFIG  CP = OFF              ; Flash Program Memory Code Protection bit (Code protection off)


    psect programa, class = CODE, reloc=2
PROGRAMA:
    ;goto    BLINK
    ;goto    INICIO_LCD
    ;goto    MAIN_LED
    ;goto    MAIN_LCD
    goto    LCD_INIT
    
    psect code
BLINK:
    bank1
    clrf    TRISA           ; Salida
    clrf    TRISD           ; Salida
    clrf    TRISC           ; Salida
    bank0
    bcf	    PORTA, 3	    ; RA3=0
    bsf	    PORTC, 5; RC5=1
    ;call    Retardo_1ms     ; Espera 1 ms
BLINK_LOOP:
    bsf	    PORTD, 0
    call    Delay2
    bcf	    PORTD, 0
    call    Delay2
    goto BLINK_LOOP
    
/*
    psect code
INICIO_LCD:
	CALL	LCD_INIT
	MOVLW	1
	CALL	LCD_LINEA1
	MOVLW	'H'
	CALL	LCD_CARACTER
	MOVLW	'O'
	CALL	LCD_CARACTER
	MOVLW	'L'
	CALL	LCD_CARACTER
	MOVLW	'A'
	CALL	LCD_CARACTER
	MOVLW	5
	CALL	LCD_LINEA2
	MOVLW	'W'
	CALL	LCD_CARACTER
	MOVLW	'E'
	CALL	LCD_CARACTER
	MOVLW	'L'
	CALL	LCD_CARACTER
	MOVLW	'S'
	CALL	LCD_CARACTER
START_LCD:
	GOTO	START_LCD
*/
	
	
    end			    ; End of the program



