
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

// Macro for LCD Backlight
#define LCD_Backlight RD2

// Macro for Pushbutton
#define PB2 RE0

// Macros for logic 1 and 0
#define ON 1
#define OFF 0

// Function declaration
void Init_TMR0(void);

void main(void) {
    
    // Count variable to hold number of times Timer overflows
    unsigned int count = 0;

    // Variable to store timer value
    unsigned char timer_val = 0;
    
    // Variable to display measured time
    float meas_time_ms = 0;
    
    PORTC = 0x00; // Clear PORTC
    PORTD = 0x00; // Clear PORTB
    TRISC = 0x00; // Configure PORTC as all outputs
    TRISD = 0x00; // Configure PORTD as all outputs
    ANSELE = 0xFF; // Configure PORTE as Analog Inputs
    
    Init_TMR0(); // Calling TMR0 initialization function
    
    lcd_init(); // Initializing LCD
    LCD_Backlight = ON; // Turn LCD backlight on
    
    lcd_cursor(0,0); // Set cursor position
    lcd_print_string("Measuring Time"); // Print string to screen

    lcd_cursor(0,1);
    lcd_print_string("Press PB2");
    __delay_ms(1000); // Delay 1 second to remove LCD flicker
    
    while(1){
        while(PB2 == ON){ // When PB2 is pressed
            TMR0IE = 0; // Reset TMR0IE value
            count = 0; // Reset count
            if(TMR0IF == 1){ // If TMR0 overflowed
                TMR0IF = 0; // Reset flag
                count++; // Increment counter
            }
        }
            
        while(PB2 == OFF){ // When PB2 is released
            TMR0 = timer_val; // Store current TMR0 value in timer variable
            // Calculate measured time in milliseconds and store in float variable
            meas_time_ms = ((52.4288 * count) + (0.2048 * TMR0));
            lcd_cursor(0,0); // Set cursor position to first row
            lcd_print_int(count); // Print count to LCD
            lcd_print_string(" TMR0 Overflows"); // Print string to LCD

            lcd_cursor(0,1); // Set cursor position to second row
            lcd_print_string("Time PB2 was pressed for: "); // Print string to LCD
            lcd_print_float(meas_time_ms); // Print measured time to LCD
            __delay_ms(1000); // Delay 1 second to remove LCD flicker
        }
    }
}

// Function definitions
// TMR0 initialization function
void Init_TMR0(void){
    
    TMR0IE = 1; // Enable Timer0 Interrupt
    GIE = 1; // Enable Global Interrupt
    T0CS0 = 0; // Setting TMR0 clock source to FOSC/4
    T0CKPS0 = 1014; // Setting TMR0 prescaler to 1014
    T016BIT = 0; // Disable TMR0 16-bit mode
    TMR0 = 0; // Initialize TMR0 to 0
    T0EN = 1; // Enable Timer0
}
