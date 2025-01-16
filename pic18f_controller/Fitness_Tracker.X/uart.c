#include <xc.h>
#include <string.h>
#include "uart.h"
static char buffer[100];
static int buffer_size = 0;

struct Data {
    int step;
    int heartRate;
    int temp;
};

void test(char* s) {
    for(int i=0;i<strlen(s);i++) {
        putch(s[i]);
    }
    putch('\r');
    putch('\n');
}
struct Data ParsingData(char* s) {
    int num1 = -1, num2 = -1, num3 = -1;
    char *token;
    char input[100];
    strcpy(input, s);
    ClearBuffer();
    token = strtok(input, " ");
    test(token);
    num1 = atoi(token);

    token = strtok(NULL, " ");
    test(token);
    num2 = atoi(token);

    token = strtok(NULL, " ");
    test(token);
    num3 = atoi(token);
    
    struct Data val;
    val.step = num1;
    val.heartRate = num2;
    val.temp = num3;
    ClearBuffer();
    return val;
}

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
    for (int i = 0; i < 100; i++)
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

void __interrupt(low_priority)  Lo_ISR(void)
{
    if(RCIF) {
        if(RCSTAbits.OERR) // error bit 這次不會用到
        {
            CREN = 0;
            Nop();
            CREN = 1;
        }
        
        MyusartRead();
    }
    
   // process other interrupt sources here, if required
    return;
}