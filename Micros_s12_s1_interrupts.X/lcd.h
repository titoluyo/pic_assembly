#include <xc.h>
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000  // 8Mhz 
#endif

#define USE_CGRAM_LCD

#define RS LATDbits.LD2
//#define RW LATDbits.LD1
#define EN LATDbits.LD3 //LD0

#define D4 LATDbits.LD4
#define D5 LATDbits.LD5
#define D6 LATDbits.LD6
#define D7 LATDbits.LD7
//---------------------------------
#define RS_DIR TRISDbits.TRISD2// TRISD0
//#define RW_DIR TRISDbits.TRISD1
#define EN_DIR TRISDbits.TRISD3//TRISD2

#define D4_DIR TRISDbits.TRISD4
#define D5_DIR TRISDbits.TRISD5
#define D6_DIR TRISDbits.TRISD6
#define D7_DIR TRISDbits.TRISD7

void L1Lcd_Port(char a);
void L1Lcd_Cmd(char a);
void L1Lcd_Clear(void);
void L1Lcd_Set_Cursor(char a, char b);
void L1Lcd_Init(void);
void L1Lcd_Write_Char(char a);
void L1Lcd_Write_String(const char *a);
void L1Lcd_Shift_Right(void);
void L1Lcd_Shift_Left(void);
void L1Lcd_Blink(void);
void L1Lcd_NoBlink(void);

#ifdef USE_CGRAM_LCD
void L1Lcd_CGRAM_CreateChar(char add, const char* chardata);
void L1Lcd_CGRAM_Init(void);
void L1Lcd_CGRAM_Close(void);
#endif