#include <xc.inc>

; Pin definition
RS  macro
    PORTC, 3
    endm
RW  macro
    PORTC, 4
    endm
E   macro
    PORTC, 5
    endm

/*
; Variables declaration in data section
psect data_section, class=UDATA, space=1
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
*/

;psect text_section, class=CODE, reloc=2
    psect code
LCD_INIT:
    ; Configurar TRISD y TRISC como salidas
    bank1
    clrf    TRISD           ; Output
    clrf    TRISC           ; Output
    bank0
    call    Delay_1ms       ; Wait 1 ms
    bcf     RS              ; Command Mode
    movlw   0x30            ; Configure data bits
    call    LCD_CONFIG
    call    Retardo_1ms
    movlw   0x30
    call    LCD_CONFIG
    call    Retardo_1ms
    movlw   0x30
    call    LCD_CONFIG
    call    Retardo_1ms
    movlw   0x20            ; 4 bits mode configuration
    call    LCD_CONFIG
    call    LCD_2LINEAS      ; 2 lines configuration
    call    LCD_DISPLAY      ; Turn on display
    call    LCD_BORRAR       ; Clear screen
    call    LCD_INCR         ; Cursor increment
    return

LCD_LINEA2:
    addlw   0xC0
    goto    LCD_COMANDO

LCD_LINEA1:
    addlw   0x80
    goto    LCD_COMANDO

LCD_DISPLAY:
    movlw   0x0C
    goto    LCD_COMANDO

LCD_BORRAR:
    movlw   0x01
    goto    LCD_COMANDO

LCD_INCR:
    movlw   0x06
    goto    LCD_COMANDO

LCD_2LINEAS:
    movlw   0x28
    goto    LCD_COMANDO

LCD_COMANDO:
    movwf   COMANDO         ; Save command
    movf    COMANDO,W       ; Load command into W register
    call    LCD_CONFIG
    swapf   COMANDO,F       ; Intercambiar bits del comando
    movf    COMANDO,W
    call    LCD_CONFIG
    return

LCD_CARACTER:
    movwf   DATA_GUARDAR    ; Guardar dato a escribir
    bsf     RS              ; Modo carácter
    call    LCD_DATOS
    swapf   DATA_GUARDAR,F
    movf    DATA_GUARDAR,W
    call    LCD_DATOS
    return

LCD_DATOS:
    andlw   11110000B     ; Aislar los bits altos
    movwf   DATAS           ; Guardar en DATAS
    bsf     RS              ; Modo carácter
    movf    PORTD,W         ; Leer PORTD
    andlw   00001111B     ; Aislar los bits bajos
    iorwf   DATAS           ; Combinar con los bits de DATAS
    movf    DATAS,W
    movwf   PORTD           ; Escribir en PORTD
    call    ENABLE
    return

LCD_CONFIG:
    andlw   11110000B	    ; Aislar los bits altos
    movwf   GUARDAR_DATO    ; Guardar el dato
    bcf     RS              ; Modo comando
    movf    PORTD,W         ; Leer PORTD
    andlw   00001111B	    ; Aislar los bits bajos
    iorwf   GUARDAR_DATO    ; Combinar con GUARDAR_DATO
    movf    GUARDAR_DATO,W
    movwf   PORTD           ; Escribir en PORTD
    call    ENABLE
    return

ENABLE:
    call    Retardo_1ms     ; Esperar 1 ms
    call    Retardo_1ms     ; Esperar 1 ms
    bsf     E		    ; Habilitar
    call    Retardo_1ms     ; Esperar 1 ms
    bcf     E               ; Deshabilitar
    return

; Subrutina de retardo de 1ms
Delay_1ms:
    movlw   250
    movwf   CUENTA
BUCLE:
    nop
    decfsz  CUENTA,F
    goto    BUCLE
    movf    DATO,W
    return

