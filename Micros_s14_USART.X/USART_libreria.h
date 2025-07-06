#ifndef USART_LIBRERIA_H
#define	USART_LIBRERIA_H

void USART_Init(long BAUD);
void USART_Tx(char data);
char USART_Rx();
void USART_Cadena(char *str);

#endif	/* USART_LIBRERIA_H */

