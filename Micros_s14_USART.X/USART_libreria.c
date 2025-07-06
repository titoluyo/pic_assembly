#include <xc.h>
#include "config.h"
#include "USART_libreria.h"

void USART_Init(long BAUD){
    //Configuraci?n de los pines
    TRISCbits.RC6 = 0;      //RC6 = Tx -> Salida
    TRISCbits.RC7 = 1;      //RC7 = Rx -> Entrad
    
    //Baudios
    SPBRG = (unsigned char)(((_XTAL_FREQ/BAUD)/64)-1);
    
    //Configuraci?n 
    TXSTAbits.BRGH = 0;     //Low Speed
    TXSTAbits.SYNC = 0;     //Asincrono
    RCSTAbits.SPEN = 1;     //Habilitar Tx y Rx
    
    //Transmisi?n
    TXSTAbits.TX9 = 0;      //8 bits
    TXSTAbits.TXEN = 1;     //Activamos transmisi?n
    
    //Recepci?n
    RCSTAbits.RC9 = 0;      //8 bits
    RCSTAbits.CREN = 1;     //Activamos recepci?n
}

void USART_Tx(char data){
    while(TXSTAbits.TRMT == 0);
    TXREG = data;
}

char USART_Rx(){
    return RCREG; 
}


void USART_Cadena(char *str){
    while(*str != '\0'){
        USART_Tx(*str);
        str++;
    }
}