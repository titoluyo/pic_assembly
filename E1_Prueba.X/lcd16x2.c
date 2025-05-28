#include "lcd16x2.h"
#include "i2c.h"
#include "lcd_i2c.h"
#include "lcd.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000  // 4Mhz 
#endif

#define LCD_I2C
#define LCD_BUS

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

Lcd_I2C lcd1 = { .address = 0x4E };
Lcd_I2C lcd2 = { .address = 0x4C };

// lcd 16x2 HD44780U
void Lcd16x2_init(void) {
    ADCON1bits.PCFG = 0x0F; // config all pins as digital
#ifdef LCD_I2C
    I2C_Init_Master(I2C_100KHZ);
    //__delay_ms(100);
    Lcd_Init(&lcd1);
    __delay_ms(100);
    Lcd_Clear(&lcd1);
    __delay_ms(100);
//    Lcd_CGRAM_Init();
//    Lcd_CGRAM_CreateChar(0, cara);
//    Lcd_CGRAM_Close();

    //__delay_ms(100);
    Lcd_Init(&lcd2);
    __delay_ms(100);
    Lcd_Clear(&lcd2);
    __delay_ms(100);
#endif
    
#ifdef LCD_BUS
    __delay_ms(100);
    L1Lcd_Init();
    __delay_ms(100);
    L1Lcd_Clear();
    __delay_ms(100);
#endif
}

void Lcd16x2_loop(void) {
    __delay_ms(100);  // wait for 3sec
    //*
#ifdef LCD_I2C
    Lcd_Set_Cursor(&lcd1, 1, 1); /// Cursor at row 1, col 1
    __delay_ms(50);  // wait for 3sec
    Lcd_Write_String(&lcd1, "Hola UTP2a!!!"); // escribe Hola UTP!!!
    __delay_ms(50);  // wait for 3sec

    Lcd_Set_Cursor(&lcd2, 1, 1); /// Cursor at row 1, col 1
    __delay_ms(50);  // wait for 3sec
    Lcd_Write_String(&lcd2, "Hola UTP2c!!!"); // escribe Hola UTP!!!
    __delay_ms(50);  // wait for 3sec
#endif

#ifdef LCD_BUS
    L1Lcd_Set_Cursor(1, 1); /// Cursor at row 1, col 1
    __delay_ms(50);  // wait for 3sec
    L1Lcd_Write_String("Hola UTP2b!!!"); // escribe Hola UTP!!!
    __delay_ms(50);  // wait for 3sec
#endif
    __delay_ms(3000);  // wait for 3sec
    
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
    
 #ifdef LCD_I2C
   Lcd_Clear(&lcd1);
    __delay_ms(50);  // wait 
    Lcd_Clear(&lcd2);
    __delay_ms(50);  // wait 
#endif
    
 #ifdef LCD_BUS
   L1Lcd_Clear();
    __delay_ms(50);  // wait 
#endif

    __delay_ms(2000);  // wait for 2 sec

}