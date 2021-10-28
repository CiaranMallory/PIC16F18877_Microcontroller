
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

#include <xc.h>

// Setting Crystal Frequency
#define _XTAL_FREQ 19660800

// Macros for LEDs
#define LED LATC0
#define SERVO LATB1

// Macros for logic 1 and 0
#define ON 1
#define OFF 0

// Count variable to hold number of times Timer overflows
unsigned int count = 0;

// Variable to pass desired pwm value from main() to ISR
unsigned char pwm_val = 0;

// Function declaration
void Init_TMR0(void);

void main(void) {
    
    PORTC = 0x00; // Clear PORTC
    PORTB = 0x00; // Clear PORTB
    TRISC = 0x00; // Configure PORTC as all outputs
    TRISB = 0x00; // Configure PORTB as all outputs
    ANSELE = 0x00; // Disable all Analog Inputs
    
    Init_TMR0(); // Calling TMR0 initialization function
    
    while(1){
        // For loop to increment pwm value from 5 to 10 in steps
        // of 1 every 2 seconds
        for(int i = 5; i < 11; i++){
            pwm_val++; // Increment pwm value
            __delay_ms(2000); // Delay 2 seconds
        }
        
        // For loop to decrement pwm value from 10 to 5 in steps
        // of 1 every 2 seconds
        for(int i = 11; i > 5; i--){
            pwm_val--; // Decrement pwm value
            __delay_ms(2000); // Delay 2 seconds
        }
    }
}

// Function definitions
// TMR0 initialization function
void Init_TMR0(void){
    
    TMR0IE = 1; // Enable Timer0 Interrupt
    GIE = 1; // Enable Global Interrupt
    T0CS0 = 0; // Setting TMR0 clock source to FOSC/4
    T0CKPS0 = 1; // Setting TMR0 prescaler to 1
    T016BIT = 1; // Enable TMR0 16-bit mode
    TMR0 = 64536; // Initialize TMR0 to 64536
    T0EN = 1; // Enable Timer0
    SERVO = ON; // Turn servo on
    LED = ON; // Turn LED on to track servo output
}

// Interrupt Function
void __interrupt() isr(void){
    if(TMR0IF == 1){ // Check for TMR0 overflow
        TMR0 = 64536; // Initialize TMR0 to 64536
        TMR0IF = 0; // Reset TMR0 overflow flag
        count++; // Increment TMR0 overflow counter
        if(count >= pwm_val){ // Check if count is greater than or equal to the pwm value
            SERVO = OFF; // Turn off servo
            LED = OFF; // Turn off LED
        }
        else if(count == 100){
            // If count equals 100(20mS)
            count = 0; // Reset count
            SERVO = ON; // Turn on servo
            LED = ON; // Turn on LED
        }
    }
}
