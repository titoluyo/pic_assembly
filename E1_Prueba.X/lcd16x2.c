#include "lcd16x2.h"
#include "lcd.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000  // 4Mhz 
#endif

const unsigned char cara[] = {
  0x1F,
  0x1F,
  0x15,
  0x1F,
  0x1F,
  0x0E,
  0x11,
  0x1F
};

// lcd 16x2 HD44780U
void Lcd16x2_init(void) {
    ADCON1bits.PCFG = 0x0F; // config all pins as digital
    Lcd_Init();
    Lcd_Clear();
//    Lcd_CGRAM_Init();
//    Lcd_CGRAM_CreateChar(0, cara);
//    Lcd_CGRAM_Close();
}

void Lcd16x2_loop(void) {
    //*
//    __delay_ms(100);  // wait for 3sec
    Lcd_Set_Cursor(1, 1); /// Cursor at row 1, col 1
    __delay_ms(300);  // wait for 3sec
    Lcd_Write_String("Hola UTP!!!"); // escribe Hola UTP!!!
    __delay_ms(3000);  // wait for 3sec
    Lcd_Clear();
    __delay_ms(2000);  // wait for 3sec
//    for (char c=0; c<=4; c++) {
//        Lcd_Shift_Right();
//        __delay_ms(300);
//    }
//    for (char c=0; c<=4; c++) {
//        Lcd_Shift_Left();
//        __delay_ms(300);
//    }
    //Lcd_Clear(); // clear screen
    //__delay_ms(1000); // wait for 1sec
    /*/
    Lcd_Set_Cursor(1,1);
    Lcd_Write_Char(0);
    __delay_ms(1000);
    Lcd_Clear();
    //*/
}