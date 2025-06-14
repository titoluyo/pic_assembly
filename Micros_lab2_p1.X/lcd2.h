#include <xc.h>
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000  // 8Mhz 
#endif

#define USE_CGRAM_LCD

#define L2RS LATBbits.LB2
#define L2RW LATBbits.LB1
#define L2EN LATBbits.LB0

#define L2D4 LATBbits.LB4
#define L2D5 LATBbits.LB5
#define L2D6 LATBbits.LB6
#define L2D7 LATBbits.LB7
//---------------------------------
#define L2RS_DIR TRISBbits.TRISB2
#define L2RW_DIR TRISBbits.TRISB1
#define L2EN_DIR TRISBbits.TRISB0

#define L2D4_DIR TRISBbits.TRISB4
#define L2D5_DIR TRISBbits.TRISB5
#define L2D6_DIR TRISBbits.TRISB6
#define L2D7_DIR TRISBbits.TRISB7

void L2Lcd_Port(char a);
void L2Lcd_Cmd(char a);
void L2Lcd_Clear(void);
void L2Lcd_Set_Cursor(char a, char b);
void L2Lcd_Init(void);
void L2Lcd_Write_Char(char a);
void L2Lcd_Write_String(const char *a);
void L2Lcd_Shift_Right(void);
void L2Lcd_Shift_Left(void);
void L2Lcd_Blink(void);
void L2Lcd_NoBlink(void);

#ifdef USE_CGRAM_LCD
void L2Lcd_CGRAM_CreateChar(char add, const char* chardata);
void L2Lcd_CGRAM_Init(void);
void L2Lcd_CGRAM_Close(void);
#endif