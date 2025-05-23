#include <xc.h>
// #define _XTAL_FREQ 48000000
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000  // 4Mhz 
#endif

#define USE_CGRAM_LCD

#define RS LATBbits.LB5 // LATDbits.LD2
#define RW LATBbits.LB4
#define EN LATBbits.LB3 // LATDbits.LD3

#define D4 LATDbits.LD4
#define D5 LATDbits.LD5
#define D6 LATDbits.LD6
#define D7 LATDbits.LD7
//---------------------------------
#define RS_DIR TRISBbits.TRISB5 // TRISDbits.TRISD2
#define RW_DIR TRISBbits.TRISB4
#define EN_DIR TRISBbits.TRISB3 // TRISDbits.TRISD3

#define D4_DIR TRISDbits.TRISD4
#define D5_DIR TRISDbits.TRISD5
#define D6_DIR TRISDbits.TRISD6
#define D7_DIR TRISDbits.TRISD7

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(const char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
void Lcd_Blink(void);
void Lcd_NoBlink(void);

#ifdef USE_CGRAM_LCD
void Lcd_CGRAM_CreateChar(char add, const char* chardata);
void Lcd_CGRAM_Init(void);
void Lcd_CGRAM_Close(void);
#endif