
// PIC16F18877 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
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
#pragma config CPD = OFF        // DataNVM code protection bit (Data EEPROM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 20e06

#define DELAY 100 // Delay macro

// Setting up macros for logic 1 and 0 to improve code readability
#define ON 1
#define OFF 0

// Macros for establishing direction of LEDs
#define Left 2
#define Right 3

void main(void) 
{
    PORTC = 0x00; // Clearing portC
    TRISC = 0x00; // Configuring portC as outputs
    
    int state; // Variable for managing direction of LEDs
    
    while(1) // Run continuously
    {
        // If (state = Left) then turn on LEDs from right to left
        if (state == Left){
            RC0 = ON;
            __delay_ms(500);
            RC0 = OFF;

            RC1 = ON;
            __delay_ms(500);
            RC1 = OFF;

            RC2 = ON;
            __delay_ms(500);
            RC2 = OFF;

            RC3 = ON;
            __delay_ms(500);
            RC3 = OFF;

            RC4 = ON;
            __delay_ms(500);
            RC4 = OFF;

            RC5 = ON;
            __delay_ms(500);
            RC5 = OFF;

            RC6 = ON;
            __delay_ms(500);
            RC6 = OFF;

            RC7 = ON;
            __delay_ms(500);
            RC7 = OFF;
            state = Right; // Switch state
        }
        
        // If (state = Right) turn on LEDs from left to right
        if (state == Right){
            RC7 = ON;
            __delay_ms(500);
            RC7 = OFF;

            RC6 = ON;
            __delay_ms(500);
            RC6 = OFF;

            RC5 = ON;
            __delay_ms(500);
            RC5 = OFF;

            RC4 = ON;
            __delay_ms(500);
            RC4 = OFF;

            RC3 = ON;
            __delay_ms(500);
            RC3 = OFF;

            RC2 = ON;
            __delay_ms(500);
            RC2 = OFF;

            RC1 = ON;
            __delay_ms(500);
            RC1 = OFF;

            RC0 = ON;
            __delay_ms(500);
            RC0 = OFF;
            state = Left; // Switch state
        }
    }
}
