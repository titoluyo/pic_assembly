#ifndef UART_H
#define	UART_H

void UART_Init(void);              // Inicializacion del UART
void Uart_SChar(char txChar);      // Envio de un caracter en el UART
void Uart_SString(char *txString); // Envio de una cadena de caracteres

#endif	/* UART_H */

