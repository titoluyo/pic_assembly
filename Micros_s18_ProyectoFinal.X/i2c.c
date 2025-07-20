#include "i2c.h" // Incluye definiciones y prototipos I2C

#ifdef I2C_MASTER_MODE

// Inicializa el modulo I2C en modo maestro
void I2C_Init_Master(unsigned char sp_i2c)
{
    TRIS_SCL = 1;                   // Configura pin SCL como entrada (requerido para I2C)
    TRIS_SDA = 1;                   // Configura pin SDA como entrada
    SSPSTAT = sp_i2c;               // Configura velocidad (100 o 400 kHz)
    SSPCON1 = 0x28;                 // Habilita I2C maestro, modo sincrono
    SSPCON2 = 0x00;                 // Reinicia opciones de control

    // Configura el valor de SSPADD segun la velocidad seleccionada
    if(sp_i2c == I2C_100KHZ){
        SSPADD = 119;               // Fosc = 48 MHz, I2C = 100kHz ? SSPADD = (Fosc / (4 * I2C)) - 1
    }
    else if(sp_i2c == I2C_400KHZ){
        SSPADD = 29;                // Para 400 kHz
    }
}

// Genera condicion de inicio I2C (START)
void I2C_Start(void)
{
    SSPCON2bits.SEN = 1;           // Habilita condicion de inicio
    while(PIR1bits.SSPIF == 0);    // Espera a que se complete
    PIR1bits.SSPIF = 0;            // Limpia bandera de interrupcion
}

// Genera condicion de parada I2C (STOP)
void I2C_Stop(void)
{
    SSPCON2bits.PEN = 1;           // Habilita condicion de parada
    while(PIR1bits.SSPIF == 0);    // Espera finalizacion
    PIR1bits.SSPIF = 0;            // Limpia bandera
}

// Genera condicion de reinicio I2C (RESTART)
void I2C_Restart(void)
{
    SSPCON2bits.RSEN = 1;          // Habilita reinicio
    while(PIR1bits.SSPIF == 0);    // Espera
    PIR1bits.SSPIF = 0;            // Limpia bandera
}

// Envia senal ACK al esclavo
void I2C_Ack(void)
{
    SSPCON2bits.ACKDT = 0;         // ACK = 0
	SSPCON2bits.ACKEN = 1;         // Habilita envio de ACK
	while(PIR1bits.SSPIF == 0){};    // Espera
    PIR1bits.SSPIF = 0;            // Limpia bandera
}

// Envia senal NACK al esclavo
void I2C_Nack(void)
{
    SSPCON2bits.ACKDT = 1;         // NACK = 1
	SSPCON2bits.ACKEN = 1;         // Habilita envio de NACK
	while(PIR1bits.SSPIF == 0){};    // Espera
    PIR1bits.SSPIF = 0;            // Limpia bandera
}

// Envia un byte al esclavo
short I2C_Write(char data)
{
    SSPBUF = data;                 // Carga el byte a enviar
    while(PIR1bits.SSPIF == 0);    // Espera confirmacion
    PIR1bits.SSPIF = 0;            // Limpia bandera
    short b_ok = SSPCON2bits.ACKSTAT; // Lee si el esclavo reconocio (ACK = 0)
    return b_ok;                   // Retorna 0 si fue exitoso
}

// Lee un byte desde el esclavo
unsigned char I2C_Read(void)
{
    SSPCON2bits.RCEN = 1;          // Habilita recepcion
    while(PIR1bits.SSPIF == 0);    // Espera finalizacion
    PIR1bits.SSPIF = 0;            // Limpia bandera
    return SSPBUF;                 // Devuelve dato recibido
}
#endif

#ifdef I2C_SLAVE_MODE

// Inicializa el I2C en modo esclavo con la direccion proporcionada
void I2C_Init_Slave(unsigned char add_slave)
{
    TRIS_SCL = 1;                  // Configura el pin SCL como entrada
    TRIS_SDA = 1;                  // Configura el pin SDA como entrada
    SSPSTAT = 0x80;                // Deteccion de condiciones de parada habilitada
    SSPADD = add_slave;            // Establece direccion del esclavo
    SSPCON1 = 0x36;                // Habilita I2C esclavo 7 bits, y la comunicacion sincolision
    SSPCON2 = 0x01;                // Configura para esclavo sin esclavo extendido
    INTCONbits.GIE = 1;            // Habilita interrupciones globales
    INTCONbits.PEIE = 1;           // Habilita interrupciones perifericas
    PIR1bits.SSPIF = 0;            // Limpia bandera de interrupcion I2C
    PIE1bits.SSPIE = 1;            // Habilita interrupcion del modulo MSSP (I2C)
}

// Verifica si ocurrio un error de lectura (colision o sobrecarga)
short I2C_Error_Read(void)
{
    SSPCON1bits.CKP = 0;           // Detiene el reloj SCL
    return ((SSPCON1bits.SSPOV) || (SSPCON1bits.WCOL)) ? 1 : 0; // Retorna 1 si hay error
}

// Retorna 1 si el maestro esta escribiendo hacia el esclavo
short I2C_Write_Mode(void)
{
    return (!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) ? 1 : 0;
}

// Retorna 1 si el maestro esta leyendo desde el esclavo
short I2C_Read_Mode(void)
{
    return (!SSPSTATbits.D_nA && SSPSTATbits.R_nW) ? 1 : 0;
}

// Limpia errores de sobrecarga y colision del esclavo
void I2C_Error_Data(void)
{
    short z;
    SSPCON1bits.CKP = 0;           // Detiene el reloj
    z = SSPBUF;                    // Lee buffer para desbloquear
    SSPCON1bits.SSPOV = 0;         // Limpia bandera de sobrecarga
    SSPCON1bits.WCOL = 0;          // Limpia bandera de colision
    SSPCON1bits.CKP = 1;           // Libera el reloj
    SSPCON1bits.SSPM3 = 0;         // Modo esclavo 7 bits
}

// Lee un dato recibido desde el maestro
unsigned char I2C_Read_Slave(void)
{
    short z;
    unsigned char dato_i2c;
    z = SSPBUF;                    // Lee para desbloquear
    while (!BF);                   // Espera hasta que se llene el buffer
    dato_i2c = SSPBUF;             // Lee el dato recibido
    SSPCON1bits.CKP = 1;           // Libera el reloj
    SSPCON1bits.SSPM3 = 0;         // Modo esclavo 7 bits
    return dato_i2c;
}

// Escribe un dato que sera enviado al maestro
void I2C_Write_Slave(char dato_i2c)
{
    short z;
    z = SSPBUF;                    // Lee para limpiar buffer
    BF = 0;                        // Limpia bit de buffer lleno
    SSPBUF = dato_i2c;             // Carga dato a transmitir
    SSPCON1bits.CKP = 1;           // Libera el reloj
    while (SSPSTATbits.BF == 1){};   // Espera hasta que el buffer este vacio
}
#endif
