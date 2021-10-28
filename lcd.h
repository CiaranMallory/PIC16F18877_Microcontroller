/************************************************************************
*                                                                       *
*  File:        lcd.h                                                   *
*  Project:     lcd                                                     *
*  Description: This program creates a number of lcd functions.         * 
*               It drives a 2 x 16 character LCD display using the      *    
* 		Hitachi 44780 chip in 4-bit mode with R/W pin   	            *
*		tied low (i.e. write mode only).                                *
*		Pin assignments are as follows:      		                    *
*       PORTx:Bit5  ----->  LCD Data bit 4                              *
*  		PORTx:Bit5  ----->  LCD Data bit 5                              *
*  		PORTx:Bit6  ----->  LCD Data bit 6                              *
*  		PORTx:Bit7  ----->  LCD Data bit 7                              *  
*  		PORTx:Bit0  ----->  LCD RS (0 = Instruction; 1 = Data)          *
*  		PORTx:Bit1  ----->  LCD E                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*  Created By:  Paul Morrow 15/11/2020                                  *
*                                                                       *
*  Versions:    1.0                                                     *
*                                                                       *
************************************************************************/

#define PIC16F18877
//#define PIC16F88

/* PIC16F18877 Development Board Pin Configuration */
#ifdef PIC16F18877
#define LCD_PORT LATD 
#define TRIS     TRISD 
#define LCD_D4   4
#define LCD_D5   5
#define LCD_D6   6
#define LCD_D7   7
#define LCD_RS   0 
#define LCD_E    1
#define _XTAL_FREQ 20e06
#endif

/* PIC16F88 EB006 Multi-programmer Board Pin Configuration */
#ifdef PIC16F88
#define LCD_PORT PORTB 
#define TRIS     TRISB 
#define LCD_D4   0
#define LCD_D5   1
#define LCD_D6   2
#define LCD_D7   3
#define LCD_RS   4 
#define LCD_E    5
#define _XTAL_FREQ 19.6608e06
#endif


#define set_bit(port,bitnum) 		port |= (1 << bitnum)
#define clear_bit(port,bitnum) 		port &= ~(1 << bitnum)

void lcd_init ( void ) ;

void lcd_clear ( void ) ;

void lcd_print_char ( unsigned char in ) ;

void lcd_print_string ( const unsigned char * message ) ;

void lcd_cursor ( unsigned char x, unsigned char y ) ;

void lcd_print_int (unsigned int number) ;

void lcd_print_float (float number, unsigned int decimalPoints) ;
