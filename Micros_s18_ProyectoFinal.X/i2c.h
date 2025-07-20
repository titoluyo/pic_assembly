#include <xc.h>                        // Libreria principal del compilador XC8
#define _XTAL_FREQ 48000000            // Frecuencia del oscilador

// Definicion de pines I2C
#define TRIS_SCL TRISBbits.TRISB1      // Pin SCL configurado como entrada
#define TRIS_SDA TRISBbits.TRISB0      // Pin SDA configurado como entrada

// Constantes para velocidades de comunicacion I2C
#define I2C_100KHZ 0x80                // Valor para configurar I2C a 100 kHz
#define I2C_400KHZ 0x00                // Valor para configurar I2C a 400 kHz

#define I2C_MASTER_MODE                // Habilita las funciones del modo maestro
//#define I2C_SLAVE_MODE               // (Comentado) Para habilitar modo esclavo

#ifdef I2C_MASTER_MODE
// Prototipos de funciones para el modo maestro I2C
void I2C_Init_Master(unsigned char sp_i2c); // Inicializa el I2C maestro con velocidad dada
void I2C_Start(void);                       // Genera condicion de inicio (START)
void I2C_Stop(void);                        // Genera condicion de parada (STOP)
void I2C_Restart(void);                     // Genera condicion de reinicio (RESTART)
void I2C_Ack(void);                         // Envia ACK al esclavo
void I2C_Nack(void);                        // Envia NACK al esclavo
unsigned char I2C_Read(void);              // Lee un byte desde el esclavo
short I2C_Write(char data);                // Escribe un byte al esclavo
#endif

#ifdef I2C_SLAVE_MODE
// Prototipos de funciones para el modo esclavo I2C
void I2C_Init_Slave(unsigned char add_slave);    // Inicializa I2C esclavo con direccion
unsigned char I2C_Read_Slave(void);             // Lee datos recibidos desde maestro
void I2C_Write_Slave(char dato_i2c);            // Envia datos al maestro
void I2C_Error_Data(void);                      // Limpia errores de colision o sobrecarga
short I2C_Write_Mode(void);                     // Retorna si maestro este escribiendo
short I2C_Read_Mode(void);                      // Retorna si maestro este leyendo
short I2C_Error_Read(void);                     // Detecta error de lectura
#endif
