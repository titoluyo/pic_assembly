#include <xc.h>
#include "config.h"
#include "USART_libreria.h"

void main(void) {
    char valor;
    USART_Init(9600);
    while(1){
        valor = USART_Rx();
        if( valor == 'w'){
            USART_Cadena("Hola :D ");
        }
        else{
            USART_Cadena("no :c ");
        }
        __delay_ms(500);
    }
}
