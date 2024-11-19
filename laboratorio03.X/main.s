#include <xc.inc>
#define SWT1	    PORTB, 0
#define SWT2	    PORTB, 1
#define LED	    PORTB, 7
#define SWT1_TRIS   TRISB, 0
#define SWT2_TRIS   TRISB, 1
#define LED_TRIS    TRISB, 7
    
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
    goto    INICIO_PULSADOR
    
    psect code
    
    psect code
INICIO_PULSADOR:
    bcf	    LED
    bcf	    STATUS, 6	; RP1=0
    bsf	    STATUS, 5	; RP0=1 ; 01 Bank1
    bcf	    LED_TRIS	; output
    bsf	    SWT1_TRIS	; input
    bsf	    SWT2_TRIS	; input
    clrf    TRISD
    bcf	    STATUS, 5	; RP0=0	; 00 Bank0
    
START:
    clrf    PORTD
    
    btfss   SWT1
    movlw   0x01
    btfsc   SWT1
    movlw   0x00
    movwf   PORTD

    movlw   0x00
    btfss   SWT2
    movlw   0x01
    
    addwf   PORTD

    ;movwf   PORTD
    btfss   PORTD, 0
    goto    APAGAR
    
    bsf	    LED
    goto    START
APAGAR:
    bcf	    LED
    goto    START
END
