
#include <xc.h>                 // Include C header file for PIC
#include <stdlib.h>             // required for itoa function
#include "lcd.h"

void lcd_raw_send ( unsigned char in, unsigned char mask )
{
    unsigned char pt ;
    
    clear_bit(LCD_PORT, LCD_D4);
    clear_bit(LCD_PORT, LCD_D5);
    clear_bit(LCD_PORT, LCD_D6);
    clear_bit(LCD_PORT, LCD_D7);
    clear_bit(LCD_PORT, LCD_RS);
    clear_bit(LCD_PORT, LCD_E);
     
    pt = ((in >> 4) & 0x0F);        // send Upper 4 bits to LCD first
    __delay_us (1) ;
    
    if (pt & 0x01)
        set_bit(LCD_PORT, LCD_D4);
    if (pt & 0x02)
        set_bit(LCD_PORT, LCD_D5);
    if (pt & 0x04)
        set_bit(LCD_PORT, LCD_D6);
    if (pt & 0x08)
        set_bit(LCD_PORT, LCD_D7);
    if (mask)
        set_bit(LCD_PORT, LCD_RS);
   
    __delay_us(1) ;
    set_bit(LCD_PORT, LCD_E);
    __delay_us(1) ;
    clear_bit(LCD_PORT, LCD_E);
    
    pt = (in & 0x0F);                 // send Lower 4 bits to LCD next
    __delay_us (1) ;
    
    clear_bit(LCD_PORT, LCD_D4);
    clear_bit(LCD_PORT, LCD_D5);
    clear_bit(LCD_PORT, LCD_D6);
    clear_bit(LCD_PORT, LCD_D7);
    clear_bit(LCD_PORT,LCD_RS);
    clear_bit(LCD_PORT,LCD_E);

    if (pt & 0x01)
        set_bit(LCD_PORT, LCD_D4);
    if (pt & 0x02)
        set_bit(LCD_PORT, LCD_D5);
    if (pt & 0x04)
        set_bit(LCD_PORT, LCD_D6);
    if (pt & 0x08)
        set_bit(LCD_PORT, LCD_D7);
    if (mask)
        set_bit(LCD_PORT, LCD_RS);

    __delay_us(1) ;
    set_bit(LCD_PORT, LCD_E);  ;
    __delay_us(1) ;
    clear_bit(LCD_PORT,LCD_E);
    
    __delay_ms( 1 ) ;
}    

/***  prints a character at the current cursor position ***/

void lcd_print_char ( unsigned char in )
{
    /*** use raw send with RS set (i.e. RSMASK = 0x10)*/
    lcd_raw_send ( in, 0x10 ) ;
}    
 
/***  sends a command to the LCD ***/
 
void lcd_command ( unsigned char in )
{
    lcd_raw_send ( in, 0 ) ;
}
 
/***  clear the display and home the cursor ***/

void lcd_clear ( void )
{
    lcd_command ( 0x01 ) ;

    /*  do extra delay here for clear function */
    __delay_ms (5) ;

}

/***  position the cursor at x,y */

void lcd_cursor ( unsigned char x, unsigned char y )
{
    if ( y==0 ) 
    {
        /* position for line 0     */
        y=0x80 ;
    }
    else 
    {
        /* position for line 1     */
        y=0xC0 ;
    }
    lcd_command ( y+x ) ;
}
  

/***  Initialize the LCD in 4 Bit Mode with 2 line display ***/

void lcd_init ( void ) 
{
    unsigned char i, t ;
    
    /*  Set PORT direction bits for LCD as Outputs */
    
    //TRIS = (TRIS & 0xC0) ;
    clear_bit(TRIS,LCD_D4);
    clear_bit(TRIS,LCD_D5);
    clear_bit(TRIS,LCD_D6);
    clear_bit(TRIS,LCD_D7);
    clear_bit(TRIS,LCD_RS);
    clear_bit(TRIS,LCD_E);
     
    /* Sequence below follows flowchart on Page 46 of HD44780 datasheet */
    /* give the LCD time to settle after power up (15mSec Required)     */
    
    __delay_ms( 15 ) ;
    
    /* Function Set (interface is 8 bits) */

    lcd_command( 0x30);

    __delay_ms( 10 ) ;
    
   /* Function Set (interface is 8 bits) */

    lcd_command( 0x30);

    __delay_ms( 10 ) ;

   /* Function Set (interface is 8 bits)				*/

    lcd_command( 0x30);
    
    __delay_ms( 10 ) ;
    
  /* Function Set (interface is 4 bits; 2 line display; 5 x 8 dots)	*/

    lcd_command( 0x28);
    
    __delay_ms( 1 ) ;

  /* Display ON/OFF (Display ON; Cursor ON; Blinking OFF)	        */

    lcd_command( 0x0C);
    
    __delay_ms( 1 ) ;

  /*  Clear Display (automatically moves cursor to home position)       */    
    
    lcd_clear () ;

  /*  Entry Mode (Move cursor to the right after write)                 */    
    
    lcd_command( 0x06);
    
    __delay_ms( 1 ) ;
}
    


/***  lcd_print accepts a pointer parameter which points at the 	*/
/***  string to be printed                                        	*/

void lcd_print_string ( const unsigned char * mess ) 
{
    unsigned char i = 0 ;
    /*  treat the pointer as an array and send bytes untilwe find an    */ 
    /*  element with 0 in it                                            */
    
    while ( mess [i] != 0 ) 
    {
        lcd_print_char ( mess[i] ) ;
        i++ ;
    }
}    


// Reverses a string 'str' of length 'len' 
void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 
  
// Converts a given integer x to string str[].  
// d is the number of digits required in the output.  
// If d is more than the number of digits in x,  
// then 0s are added at the beginning. 
int intToStr(unsigned int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 

// Converts a floating-point/double number to a string. 
void fltoa(float n, char* res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 1); 
  
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter  
        // is needed to handle cases like 233.007 
        //fpart = fpart * pow(10, afterpoint); 
	for(unsigned char j=0; j<afterpoint; j++)
	     fpart = fpart * 10;
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 

// Prints an integer number to LCD Screen by converting integer to string
void lcd_print_int (unsigned int number)
{
     unsigned char buf [5];
     //itoa(buf,number, 10);    //depracted in C99 Compiler 
     intToStr(number, buf, 1);
     lcd_print_string(buf);
}

// Prints a floating point number to LCD Screen by converting number to string
void lcd_print_float (float number, unsigned int decimalPoints)
{
     char numStr[10];
     fltoa(number, numStr,decimalPoints);
     lcd_print_string(numStr);
}
