
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

// LCD Backlight macro
#define LCD_Backlight RD2

// Macros for enable and direction indicators
#define ENABLE RA2
#define DIR RA3

// Macros for Start and Stop
#define START RE0
#define STOP RB0

// Macros for logic 1 and 0
#define ON 1
#define OFF 0

// Function declarations
void Init_TMR2(void);
void Init_Switches(void);
void Init_ADC(void);
void Init_PWM1(void);
unsigned adc_read(void);
void pwm1_reset(void);
void pwm1_update(void);
void pwm2_reset(void);
void pwm2_update(void);
void lcd_print_duty(float float_val);

void main(void) {
    
    PORTD = 0x00; // Clear PORTD
    PORTA = 0x00; // Clear PORTA
    PORTB = 0x00; // Clear PORTB
    
    TRISA4 = 0x00; // Configure RA4 as an output
    TRISB4 = 0x00; // Configure RB4 as an output
    TRISD = 0x00; // Configure PORTD as all outputs
    
    // Reset RA4 and RB4 outputs
    LATA4 = 0x00;
    LATB4 = 0x00;

    // Calling Initialisation Functions
    Init_Switches();
    Init_ADC();
    Init_TMR2();
    Init_PWM1();
    Init_PWM2();
    
    // Variables used to print to screen
    unsigned int adc_val = 0;
    unsigned float duty_cycle = 0;
    
    lcd_init(); // Initialise LCD
    lcd_clear(); // Clear LCD screen
    
    LCD_Backlight = ON; // Turn LCD Backlight on
    
    while(1){ // Run continuously
        lcd_clear(); // Clear LCD
        lcd_print_string("MOTOR OFF"); // Print string to LCD
        pwm1_reset(); // Calling pwm1 reset function
        pwm2_reset(); // Calling pwm2 reset function
        
        while(ENABLE == OFF); // Run while ENABLE equals OFF
        
        while(ENABLE == ON){ // Run while ENABLE equals ON
            // Call ADC read function and store result in adc_val variable
            adc_val = adc_read();
            if(DIR == ON){ // If direction variable is high
                pwm1_update(); // Call pwm1 update function
                pwm2_reset(); // Call pwm2 reset function
                lcd_cursor(0,0); // Set cursor position to first line of LCD
                lcd_print_string("MOTOR FORWARDS"); // Print string to LCD
            }
            else{ // If direction variable is low
                // Notice how function calls are opposite to when
                // direction is high
                pwm2_update(); // Calling pwm2 update function
                pwm1_reset(); // Calling pwm1 reset function
                lcd_cursor(0,0); // Set cursor position to first line of LCD
                lcd_print_string("MOTOR BACKWARDS"); // Print string to LCD
            }
            
            duty_cycle = ((float)adc_val*100)/1023; // Calculate duty cycle
            lcd_print_duty(duty_cycle); // Print duty cycle to LCD
        }
    }
}

// Function definitions
// Switch Initialisation Function
void Init_Switches(void){
    // Set as digital inputs
    RA2 = 0xFF;
    RA3 = 0xFF;
}

// ADC Initialisation Function
void Init_ADC(void){
    ANSE0 = 0xFF;
    // Set ADC Clock select to use dedicated oscillator
    // Set ADC to store result as left justified
    // Set ADC Channel 
    ADON = 1; // Enable ADC
}

// TMR2 Initialization Function
void Init_TMR2(void){
    
    T0CS0 = 0; // Setting TMR0 clock source to FOSC/4
    T0CKPS0 = 1; // Setting TMR0 prescaler to 1
    TMR2 = 255; // Set TMR2 period to 255
    TMR2ON = 1; // Enable Timer2
}

// PWM1 Initialisation Function
void Init_PWM1(void){
    RC1PPS = 0x0A; // Set PPS (peripheral pin select) output signal for RC1 as CCP2
    TRISC1 = 0x00; // Set RC1 as an output pin
    // Set CCP2 Mode as PWM to 255
    CCP2FMT; // Set CCP2 as left aligned
    CCP2EN = 1; // Enable CCP2 module
}

// Function to read ADC
unsigned adc_read(void){
    unsigned int result = 0; // Variable to return to calling function
    ADGO = 1; // Start ADC conversion
    // Wait for ADC conversion to finish
    result = (ADRESH << 2) + (ADRESL >> 6); // Aligns the data correctly into a 10bit result
    return result; // Return the result
}

// PWM1 reset Function
void pwm1_reset(void){
    CCPR1L = 0; // Set PWM duty cycle to 0
}

// PWM1 update Function
void pwm1_update(void){
    // Copy ADC register result values into CCPR1x registers
    CCPR1H = ADRESH;
    CCPR1L = ADRESL & 0b11000000;
}

// PWM2 reset Function
void pwm2_reset(void){
    CCPR2L = 0; // Set PWM duty cycle to 0
}

// PWM2 update Function
void pwm2_update(void){
    // Copy ADC register result values into CCPR2x registers
    CCPR2H = ADRESH;
    CCPR2L = ADRESL & 0b11000000;
}

// Function to print duty cycle to LCD
void lcd_print_duty(float float_val)
{
    lcd_cursor(0, 1); // move lcd cursor to start of line 2
    lcd_print_string(" Duty(%)= "); // print string to LCD
 
    if (float_val < 10) // check if float_val has one digit
    {
        lcd_print_string(" "); // pad with 2 spaces
    }
    else if (float_val < 100) // check if float_val has two digit
    {
        lcd_print_string(" "); // pad with 1 space
    }
    else // otherwise float_val has > two digits
    {
        lcd_print_string(""); // pad with 0 space
    }
    
    lcd_print_float(float_val, 1); // print float value with 1 decimal place
}
