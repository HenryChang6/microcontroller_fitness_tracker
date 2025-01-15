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
#include "uart.h"
#include "init.h"
#include "parsing.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h>
#define _XTAL_FREQ 4000000
#define STR_MAX 100
#define VR_MAX ((1 << 10) - 1)
// #define delay(t) __delay_ms(t * 1000);

bool btn_interr = false;
int cnt = 0;
struct Data {
    int step;
    int heartRate;
    int temp;
};
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
void Start_Timer() {
    cnt = 0;
}
void Get_Timer_H(char* str) {
    int result = cnt / 42;
    int h = result / 60;
    char str2[100];
    itoa(h, str2);
    if( h < 10 ) strcpy(str, "0");
    strcat(str, str2);
}
void Get_Timer_M(char* str) {
    int result = cnt / 42;
    int m = result % 60;
    char str2[100];
    itoa(m, str2);
    if( m < 10 ) strcpy(str, "0");
    strcat(str, str2);  
}
void main() {
    
    Initialize();
    char str[STR_MAX];
    ClearBuffer();
    
    while (1) {
        if( GetString(str) ) {
            struct Data return_val = ParsingData(str);
        }
        if (ADCON0bits.GO == 0) ADCON0bits.GO = 1;
    }
}