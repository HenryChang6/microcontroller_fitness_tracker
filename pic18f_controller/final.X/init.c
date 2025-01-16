/*
 * File:   init.c
 * Author: jyunanchen
 *
 * Created on January 16, 2025, 3:50 AM
 */


#include <xc.h>
#include "init.h"

void Initialize(void) {
    // Configure oscillator
    OSCCONbits.IRCF = 0b110;  // 4 MHz
    // Configure ADC
    TRISAbits.RA0 = 1;         // Set RA0 as input port
    ADCON1bits.PCFG = 0b1110;  // AN0 as analog input
    ADCON0bits.CHS = 0b0000;   // Select AN0 channel
    ADCON1bits.VCFG0 = 0;      // Vref+ = Vdd
    ADCON1bits.VCFG1 = 0;      // Vref- = Vss
    ADCON2bits.ADCS = 0b000;   // ADC clock Fosc/2
    ADCON2bits.ACQT = 0b001;   // 2Tad acquisition time
    ADCON0bits.ADON = 1;       // Enable ADC
    ADCON2bits.ADFM = 1;       // Right justified

    // Configure servo (PWM)
    T2CONbits.TMR2ON = 0b1;      // Timer2 on
    T2CONbits.T2CKPS = 0b11;     // Prescaler 16
    CCP1CONbits.CCP1M = 0b1100;  // PWM mode
    PR2 = 0x9b;                  // Set PWM period

    TRISCbits.TRISC2 = 0;

    // Configure I/O ports
    TRISA &= 0xF1;  // Set RA1-RA3 as outputs for LED
    TRISB = 1;      // RB0 as input for button
    TRISC = 0;      // PORTC as output for servo
    LATA &= 0xF1;   // Clear RA1-RA3
    LATC = 0;       // Clear PORTC

    // Configure interrupts
    INTCONbits.INT0IF = 0;  // Clear INT0 flag
    INTCONbits.INT0IE = 1;  // Enable INT0 interrupt
    PIE1bits.ADIE = 1;      // Enable ADC interrupt
    PIR1bits.ADIF = 0;      // Clear ADC flag
    INTCONbits.PEIE = 1;    // Enable peripheral interrupt
    INTCONbits.GIE = 1;     // Enable global interrupt
    RCONbits.IPEN = 1;      // enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    // enable high priority interrupt
    INTCONbits.GIEL = 1;    // disable low priority interrupt

    // Configure UART
    /*
           TODObasic
           Serial Setting
        1.   Setting Baud rate
        2.   choose sync/async mode
        3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
        3.5  enable Tx, Rx Interrupt(optional)
        4.   Enable Tx & RX
    */
    TRISCbits.TRISC6 = 1;  // RC6(TX) : Transmiter set 1 (output)
    TRISCbits.TRISC7 = 1;  // RC7(RX) : Receiver set 1   (input)

    // Setting Baud rate
    // Baud rate = 1200 (Look up table)
    TXSTAbits.SYNC = 0;     // Synchronus or Asynchronus
    BAUDCONbits.BRG16 = 0;  // 16 bits or 8 bits
    TXSTAbits.BRGH = 0;     // High Baud Rate Select bit
    SPBRG = 51;             // Control the period

    // Serial enable
    RCSTAbits.SPEN = 1;  // Enable asynchronus serial port (must be set to 1)
    PIR1bits.TXIF = 0;   // Set when TXREG is empty
    PIR1bits.RCIF = 0;   // Will set when reception is complete
    TXSTAbits.TXEN = 1;  // Enable transmission
    RCSTAbits.CREN = 1;  // Continuous receive enable bit, will be cleared when error occured
    PIE1bits.TXIE = 0;   // Wanna use Interrupt (Transmit)
    IPR1bits.TXIP = 0;   // Interrupt Priority bit
    PIE1bits.RCIE = 1;   // Wanna use Interrupt (Receive)
    IPR1bits.RCIP = 0;   // Interrupt Priority bit
    /* Transmitter (output)
     TSR   : Current Data
     TXREG : Next Data
     TXSTAbits.TRMT will set when TSR is empty
    */
    /* Reiceiver (input)
     RSR   : Current Data
     RCREG : Correct Data (have been processed) : read data by reading the RCREG Register
    */

    // Start ADC conversion
    ADCON0bits.GO = 1;
    
    // Configure Timer2
    T2CON = 0b01111111; // Prescaler 1:16, Postscaler 1:16, Timer2 ON
    PR2 = 122;          // Set Timer2 period to achieve 0.5s delay
    PIR1bits.TMR2IF = 0; // Clear Timer2 interrupt flag
    PIE1bits.TMR2IE = 1; // Enable Timer2 interrupt
    IPR1bits.TMR2IP = 1; // Set Timer2 interrupt as high priority
}
