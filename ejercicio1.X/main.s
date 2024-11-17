#include <xc.inc>

#define LED 5

;; otra forma
;LED macro
;    5
;    endm

    psect programa, class = CODE, reloc=2
PROGRAMA:
    goto    INICIO
    
    psect code
INICIO:
    bsf	    STATUS, 5	; RP0=1, Cambia a BANCO1 (bit RP0 = 5)
    bcf	    TRISB, LED	; Configura pin del LED como salida 
    bcf	    STATUS, 5	; RP0=0, Vuelve a BANCO0
REPITE:
    bsf	    PORTB, LED	; Enciende el LED 
    call    DELAY	; Retardo
    bcf	    PORTB, LED	; ApagaelLED 
    call    DELAY	; Retardo
    goto    REPITE
DELAY:
    nop
    nop
    return
END
