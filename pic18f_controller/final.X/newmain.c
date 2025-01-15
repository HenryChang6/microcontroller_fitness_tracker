// CONFIG1H
#pragma config OSC = INTIO67  // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF    // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON      // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF  // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1  // Watchdog Timer Postscale Select bits (1:1)

// CONFIG3H
#pragma config CCP2MX = PORTC  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF   // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON      // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON  // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF    // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF  // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF  // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF  // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF  // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF  // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF  // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF  // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF  // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF  // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF  // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF  // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF  // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF  // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF  // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF  // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF  // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF  // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF  // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF  // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

#include <ctype.h>
#include <pic18f4520.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h>
#define _XTAL_FREQ 4000000
#define STR_MAX 100
#define VR_MAX ((1 << 10) - 1)
// #define delay(t) __delay_ms(t * 1000);

char buffer[STR_MAX];
int buffer_size = 0;
bool btn_interr = false;
int cnt = 0;
// ---------------- Uart --------------------

void putch(char data) {  // Output on Terminal
    if (data == '\n' || data == '\r') {
        while (!TXSTAbits.TRMT);
        TXREG = '\r';
        while (!TXSTAbits.TRMT);
        TXREG = '\n';
    } else {
        while (!TXSTAbits.TRMT);
        TXREG = data;
    }
}

void ClearBuffer() {
    for (int i = 0; i < STR_MAX; i++)
        buffer[i] = '\0';
    buffer_size = 0;
}

void MyusartRead() {
    char data = RCREG;
    if (!isprint(data) && data != '\r') return;
    buffer[buffer_size++] = data;
    putch(data);
}

int GetString(char *str) {
    if (buffer[buffer_size - 1] == '\r') {
        buffer[--buffer_size] = '\0';
        strcpy(str, buffer);
        ClearBuffer();
        return 1;
    } else {
        str[0] = '\0';
        return 0;
    }
}

void __interrupt(low_priority) Lo_ISR(void) {
    if (RCIF) {
        if (RCSTAbits.OERR) {
            CREN = 0;
            Nop();
            CREN = 1;
        }

        MyusartRead();
    }

    // process other interrupt sources here, if required
    return;
}

// ---------------- Settings --------------------

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
void __interrupt(high_priority) H_ISR() {

    if (INTCONbits.INT0IF) {  // Handle button interrupt
        __delay_ms(50);  // bouncing problem
        btn_interr = true;
        INTCONbits.INT0IF = 0;
    }
    
    if (PIR1bits.TMR2IF) {
        cnt++;
        PIR1bits.TMR2IF = 0; // Clear the interrupt flag
        
    }
}

int delay(double sec) {
    btn_interr = false;
    for (int i = 0; i < sec * 1000 / 10; i++) {
        if (btn_interr) return -1;
        __delay_ms(10);
    }
    return 0;
}


void Start_Timer() {
    cnt = 0;
}
int End_Timer() {
   return cnt / 42; 
}
void reverse(char *first, char *last) {
    for (;first < last; first++,last--) {
        // swap
        char tmp = *first;
        *first = *last;
        *last = tmp;
    }
}

void itoa(int n, char *s) {
    int sign = 1;
    if (n < 0) {
        sign = -1;
        n = -n;
    }
    int i = 0;
    while (n != 0) {
        s[i++] = n%10 + '0';
        n = n/10;
    }

    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';

    reverse(s, s+i-1);
}
int get_StepLength() {
    char ask[100];
    strcpy(ask, "Input Step Length: ");
    for(int i=0;i<strlen(ask);i++) putch(ask[i]);
    int input = 0;
    char str[STR_MAX];
    ClearBuffer();
    while( input == 0 ) {
        for(int i=0;i<strlen(str);i++) {
            ClearBuffer();
            if( str[i] >= '0' && str[i] <= '9' ) {
                input *= 10, input += str[i] - '0';
            }
        }
    }
    
    if( input == 0 ) input = 45;
    return input;
}
void output_total_dis(int num, int step) {
    char output[100] = {};
    strcpy(output, "Total Length: ");
    for(int i=0;i<strlen(output);i++) putch(output[i]);
    char tmp[100] = {};
    itoa(num * step, tmp);
    for(int i=0;i<strlen(tmp);i++) putch(tmp[i]);
    putch('\r');
    putch('\n');
    return;
}
void print_number(int num) {
    char tmp[100];
    itoa(num, tmp);
    for(int i=0;i<strlen(tmp);i++) putch(tmp[i]);
    putch('\r');
    putch('\n');
}
void ParsingData() {
    char str[STR_MAX];
    ClearBuffer();
    int step = -1, heartRate = -1, temp = -1;
    while( GetString(str) && ( step == -1 || heartRate == -1 || temp == -1 ) ) {
        char *token;
        char input[STR_MAX];
        strcpy(input, str);
        ClearBuffer();
        token = strtok(input, " ");
        step = atoi(token);
        
        token = strtok(NULL, " ");
        heartRate = atoi(token);
        
        token = strtok(NULL, " ");
        temp = atoi(token);
    }
    print_number(step);
    print_number(heartRate);
    print_number(temp);
    
    return;
}
void main() {
    Initialize();

    char str[STR_MAX];
    ClearBuffer();
    
    while (1) {
        // Do sth in main
        ParsingData();
        if (ADCON0bits.GO == 0) ADCON0bits.GO = 1;
    }
}