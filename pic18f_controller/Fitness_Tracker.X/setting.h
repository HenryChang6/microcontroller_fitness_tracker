#ifndef SETTING_H
#define	SETTING_H
#include <xc.h>
#include <pic18f4520.h>
#include "oled.h"

void SYSTEM_Initialize(void);
void OSCILLATOR_Initialize(void);
void UART_Initialize(void);
void INTTERUPT_Initialize(void); // button interrupt
void TIMER_Initialize(void);
void OLED_Initialize(void);

#endif