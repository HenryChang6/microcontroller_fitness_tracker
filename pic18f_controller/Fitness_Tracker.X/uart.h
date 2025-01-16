#ifndef _UART_H
#define _UART_H
      
void UART_Initialize(void);
int GetString(char* str);
void UART_Write(unsigned char data);
void UART_Write_Text(char* text);
void ClearBuffer();
void MyusartRead();
void putch(char data);
struct Data ParsingData(char* s);

#endif