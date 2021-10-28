
#pragma config FEXTOSC = HS     // External Oscillator mode selection bits (HS (crystal oscillator) above 4MHz; PFM set to high power)
// RSTOSC = No Setting
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = OFF      // Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed by user software)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (FSCM timer disabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF      // Brown-out reset enable bits (Brown-out reset disabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = OFF    // Peripheral Pin Select one-way control (The PPSLOCK bit can be set and cleared repeatedly by software)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will not cause a reset)

// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled, SWDTEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4
#pragma config WRT = OFF        // UserNVM self-write protection bits (Write protection off)
#pragma config SCANE = not_available// Scanner Enable bit (Scanner module is not available for use)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (High Voltage on MCLR/Vpp must be used for programming)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (Program Memory code protection disabled)
#pragma config CPD = OFF

// Including appropriate header files
#include <xc.h>
#include "lcd.h"

// Setting crystal frequency
#define _XTAL_FREQ 20e06

// Delay macro
#define delay 500

// LED macro
#define  LED LATC0

// LED backlight macro
#define LCD_Backlight RD2

// Macros for logic 1 and 0
#define ON 1
#define OFF 0

void main(void) {
    
    // Port Initialisation
    ANSELE = 0b0000110;
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    TRISE = 0b11111111;

    ADCON0 = 0b10010000;
    ADPCH = 0b100001;

    lcd_init(); // Ensure LCD screen is clear
    LCD_Backlight = ON; // Turn LCD backlight on

    float vin; // Float variable to store vin value

    while(1) // Run continuously
    {

        ADCON0bits.ADGO = 1;
        while(ADCON0bits.ADGO==1);
        vin = ((float)ADRESH *5) /255; // Calculation to convert ADRESH value to volts

        lcd_clear(); // Ensure LCD screen is clear
        lcd_cursor(0,0); // Set cursor position
        lcd_print_string("ADH= "); // Print string to screen
        lcd_print_int(ADRESH); // Print int ADRESH value to screen
        lcd_cursor(0,1);
        lcd_print_string("vin= ");
        lcd_print_float(vin,2); // Print float vin value to screen
        __delay_ms(1000); // Delay 1 second

        // If vin is greater than 2.5V
        if (vin > 2.5)
        {
            // Turn LED on
            LED = ON;
        }
        else
        {
           // Otherwise, Turn LED off
           LED = OFF;
        };
    }
    
    return;
}

