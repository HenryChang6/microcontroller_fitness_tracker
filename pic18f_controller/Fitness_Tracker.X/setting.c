// CONFIG1H
#pragma config OSC = INTIO67      // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1        // Watchdog Timer Postscale Select bits (1:1)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

#define OLED_ADDRESS 0x3C 

#include "setting.h"

void SYSTEM_Initialize(void)
{
    OSCILLATOR_Initialize(); //default 1Mhz
    UART_Initialize();
    INTTERUPT_Initialize();
    OLED_Initialize();
    TIMER_Initialize();
}

void OSCILLATOR_Initialize(void)
{
    // default setting 4M Hz
    IRCF2 = 1; 
    IRCF1 = 1;
    IRCF0 = 0;
    // RCON = 0x0000;
}

void UART_Initialize(void) {
    // Setting UART's pin       
    TRISCbits.TRISC6 = 1;  // RC6 = TX. async serial receive data ouput          
    TRISCbits.TRISC7 = 1;  // RC7 = RX. async serial receive data input      
    // 4MHz 
    TXSTAbits.SYNC = 0;     // choose async instead of sync           
    BAUDCONbits.BRG16 = 0;  // choose 8 bit         
    TXSTAbits.BRGH = 0;
    SPBRG = 51;             // for baud rate 1200     

    RCSTAbits.SPEN = 1;  // enable asychronous serial port        
    PIR1bits.TXIF = 0;   
    PIR1bits.RCIF = 0;   // (Flagbit: set when reception is complete) 
    TXSTAbits.TXEN = 1;  // enable transmission           
    RCSTAbits.CREN = 1;  // continous receive enable bit. Will be cleared when error (OERR FERR) occured. But no error will occur in this lab.        
    // PIE1bits.TXIE = 1;    // Enable TX interrupt      
    // IPR1bits.TXIP = 0;    // Set TX interrupt as high priority interrupt
    /* Set RCIE RCIP if want to use interrupt */              
    PIE1bits.RCIE = 1;  // Enable RX interrupt           
    IPR1bits.RCIP = 0;  // Set RX interrupt as low priority interrupt 
}

void INTTERUPT_Initialize(void) { // button interrupt
    TRISBbits.RB0 = 1; // set RB0 to input

    INTCONbits.INT0IF = 0;  // Clear INT0 flag
    INTCONbits.INT0IE = 1;  // Enable INT0 interrupt
    PIE1bits.ADIE = 1;      // Enable ADC interrupt
    PIR1bits.ADIF = 0;      // Clear ADC flag
    INTCONbits.PEIE = 1;    // Enable peripheral interrupt
    INTCONbits.GIE = 1;     // Enable global interrupt
    RCONbits.IPEN = 1;      // enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    // enable high priority interrupt
    INTCONbits.GIEL = 1;    // disable low priority interrupt
}

void TIMER_Initialize(void) { // Timer Interrupt
    T2CON = 0b01111111; // Prescaler 1:16, Postscaler 1:16, Timer2 ON
    PR2 = 122;          // Set Timer2 period to achieve 0.5s delay
    PIR1bits.TMR2IF = 0; // Clear Timer2 interrupt flag
    PIE1bits.TMR2IE = 1; // Enable Timer2 interrupt
    IPR1bits.TMR2IP = 1; // Set Timer2 interrupt as high priority
}
void  OLED_Initialize(void) {
    OSCCON = 0b11110000;
    OSCTUNE = 0x00;
    
    TRISC = 0b00111111;
    LATC = 0;
    
    //Configure Button Interrupt
    ADCON1 = 0x0F;
    TRISB = 0b00000001;
    RCONbits.IPEN = 1;
    INTCONbits.INT0IF = 0;
    INTCONbits.GIE = 1;
    INTCONbits.INT0IE = 1;
    
    //I2C Initialization
    SSPCON1 = 0x28; // Enable I2C Master mode
    SSPCON2 = 0x00; // Reset SSPCON2
    SSPADD = 19; // Set I2C clock speed to 100 kHz
    SSPSTAT = 0x00; // Slew rate disabled
    
    //OLED Initialization
    I2C_Start();
    I2C_Write(OLED_ADDRESS << 1); // Write mode
    I2C_Write(0x00);              // Control byte for commands
    I2C_Write(0xAE);              // Display OFF
    I2C_Write(0xD5);              // Set display clock divide ratio
    I2C_Write(0x80);              // Max frequency
    I2C_Write(0xA8);              // Set multiplex ratio
    I2C_Write(0x1F);
    I2C_Write(0xD3);              // Set display offset
    I2C_Write(0x00);              // Offset 0
    I2C_Write(0x40);              // Set start line address
    I2C_Write(0x8D);              // Enable charge pump
    I2C_Write(0x14);
    I2C_Write(0x20);              // Set Memory Addressing Mode
    I2C_Write(0x01);              
    I2C_Write(0xA0);
    I2C_Write(0xC0);
    I2C_Write(0xDA);              // Set COM pins hardware configuration
    I2C_Write(0x02);
    I2C_Write(0x81);              // Set contrast control
    I2C_Write(0x8F);
    I2C_Write(0xD9);              // Set pre-charge period
    I2C_Write(0xF1);              // Pre-charge period
    I2C_Write(0xDB);              // Set Vcomh deselect level
    I2C_Write(0x20);
    I2C_Write(0xA4);              // Output follows RAM content
    I2C_Write(0xA6);              // Normal display
    I2C_Write(0x2E);              // Deactivate scroll
    I2C_Write(0xAF);              // Display ON
    I2C_Stop();
}
