/*
 * File:   main.c
 * Author: Colten
 *
 * Created on 2025?1?16?, ?? 7:18
 */


#include <xc.h>
#include "timer.h"
#include "setting.h"
#include "uart.h"
#include <stdint.h> 
#include <string.h>
#define _XTAL_FREQ 8000000 // Define the oscillator frequency (8 MHz)
struct Data {
    int step;
    int heartRate;
    int temp;
};
//bitmap

//heart
//record which heart to print
int which_heart = 1;
const uint8_t heart1[] = {0xF0, 0x00, 0x08, 0x01, 0x04, 0x06, 0x02, 0x18, 0x02, 0x20, 0x04, 0x40, 0x08, 0x40, 0x10, 0x80, 0x10, 0x80, 0x08, 0x40, 0x04, 0x40, 0x02, 0x20, 0x02, 0x18, 0x04, 0x06, 0x08, 0x01, 0xF0, 0x00};
const uint8_t heart2[] = {0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x10, 0x03, 0x08, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x04, 0x10, 0x03, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00};

//foots
const uint8_t foots[] = {0x40, 0x00, 0x18, 0x00, 0xD8, 0x01, 0xE6, 0x03, 0xF6, 0x0F, 0xF0, 0x1F, 0xF7, 0x1F, 0xF7, 0x39, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x1C, 0xE0, 0x9C, 0xF1, 0xDC, 0x7B, 0xE0, 0x3F, 0xEC, 0x1F, 0xCC, 0x0F, 0x80, 0x03, 0x18, 0x00, 0x58, 0x00};

//for clock
const uint8_t clock[] = {0x00, 0x00, 0x80, 0x07, 0x60, 0x18, 0x10, 0x20, 0x08, 0x40, 0x08, 0x40, 0x04, 0x80, 0xE7, 0x83, 0x07, 0x82, 0x04, 0x82, 0x08, 0x42, 0x08, 0x40, 0x10, 0x20, 0x60, 0x18, 0x80, 0x07, 0x00, 0x00};
const uint8_t dots[] = {0x00, 0x00, 0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00, 0x00, 0x00};

//for temperature
const uint8_t thermometer[] = {0x00, 0x7F, 0x80, 0x80, 0x80, 0x80, 0x80, 0x98, 0x00, 0x99, 0x80, 0x84, 0x40, 0x82, 0x20, 0x89, 0x90, 0x74, 0x08, 0x02, 0x04, 0x01, 0x82, 0x00, 0x41, 0x00, 0x21, 0x00, 0x11, 0x00, 0x0F, 0x00};
const uint8_t temp[] = {0x3C, 0x00, 0x3C, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0x3C, 0x00, 0x3C, 0x1F, 0x80, 0x31, 0xC0, 0x60, 0x60, 0xC0, 0x60, 0xC0, 0x60, 0xC0, 0xC0, 0x60, 0x80, 0x31, 0x00, 0x00};

//character
//number
const uint8_t number[][24] = {
    //0
    {0xF0, 0x0F, 0xFC, 0x3F, 0x0E, 0x70, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x0E, 0x70, 0xFC, 0x3F, 0xF0, 0x0F},
    //1
    {0x00, 0xC0, 0x30, 0xC0, 0x30, 0xC0, 0x0C, 0xC0, 0x0C, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0},
    //2
    {0x0C, 0xF0, 0x0C, 0xF0, 0x03, 0xCC, 0x03, 0xCC, 0x03, 0xC3, 0x03, 0xC3, 0x03, 0xC3, 0x03, 0xC3, 0xC3, 0xC0, 0xC3, 0xC0, 0x3C, 0xC0, 0x3C, 0xC0},
    //3
    {0x0C, 0x30, 0x0C, 0x30, 0x03, 0xC0, 0x03, 0xC0, 0x83, 0xC1, 0x83, 0xC1, 0x83, 0xC1, 0x83, 0xC1, 0x83, 0xC1, 0x83, 0xC1, 0x7C, 0x3E, 0x7C, 0x3E},
    //4
    {0x00, 0x0F, 0xC0, 0x0F, 0xF0, 0x0C, 0x3C, 0x0C, 0x0C, 0x0C, 0x0F, 0x0C, 0x03, 0x0C, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x0C},
    //5
    {0xFF, 0x30, 0xFF, 0x70, 0x83, 0x60, 0x83, 0xC0, 0xC3, 0x80, 0xC3, 0x80, 0xC3, 0x80, 0xC3, 0x80, 0x83, 0xC1, 0x83, 0x61, 0x03, 0x77, 0x00, 0x1C},
    //6
    {0xF0, 0x1F, 0xFC, 0x3F, 0x0C, 0x63, 0x83, 0x63, 0xC3, 0xC0, 0xC3, 0xC0, 0xC3, 0xC0, 0xC3, 0xC0, 0x83, 0x61, 0x8E, 0x61, 0x0C, 0x3F, 0x00, 0x1E},
    //7
    {0x0F, 0x00, 0x3F, 0x00, 0x33, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0xC0, 0x03, 0xF0, 0x03, 0x3C, 0x03, 0x0F, 0xC3, 0x03, 0xFF, 0x00, 0x3F, 0x00},
    //8
    {0x3C, 0x3C, 0x3C, 0x3C, 0x63, 0xC3, 0x63, 0xC3, 0xC3, 0xC1, 0xC3, 0xC1, 0xC3, 0xC1, 0xC3, 0xC1, 0x63, 0xC3, 0x63, 0xC3, 0x3C, 0x3C, 0x3C, 0x3C},
    //9
    {0x7C, 0x18, 0xFE, 0x70, 0x83, 0xC1, 0x83, 0xC1, 0x83, 0xC1, 0x83, 0xC1, 0x83, 0xC1, 0x83, 0xC1, 0x83, 0xC1, 0x83, 0xC1, 0xFE, 0x7F, 0xF8, 0x1F},
};
const uint8_t dot[] = {0x00, 0xC0, 0x00, 0xC0};

//alphabet
const uint8_t b[] = {0x00, 0x00, 0x00, 0xFF, 0x00, 0x89, 0x00, 0x89, 0x00, 0x89, 0x00, 0x76, 0x00, 0x00, 0x00, 0x00};
const uint8_t p[] = {0x00, 0xFF, 0x00, 0x09, 0x00, 0x09, 0x00, 0x09, 0x00, 0x09, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00};
const uint8_t m[] = {0x00, 0xFF, 0x00, 0x02, 0x00, 0x0C, 0x00, 0x10, 0x00, 0x10, 0x00, 0x0C, 0x00, 0x02, 0x00, 0xFF};
char state = '0';
char pre_state = '9';
void __interrupt(high_priority) H_ISR() { 
    if (PIR1bits.TMR2IF) {
        update_time();
        PIR1bits.TMR2IF = 0; // Clear the interrupt flag
        
    }
    if(INTCONbits.INT0IF) {
        if(state == '0')state = '1';
        else if(state == '1')state = '2';
        else if(state == '2')state = '3';
        else if(state == '3')state = '4';
        else if(state == '4')state = '0';
    
        INTCONbits.INT0IF = 0;
        __delay_ms(500);
    }
}
void main(void) {
    ClearBuffer();
    SYSTEM_Initialize();
    int last_step = 0, last_heart = 100;
    while(1) {
        char str[100];
        struct Data val;
        if( GetString(str) ) {
            val = ParsingData(str);
        }

        //Main loop
        //check state change and clean screen
        if(pre_state != state) OLED_Clear(), init_time();    // Clear OLED screen
        pre_state = state;
        char tmp[100] = {};
        char hh[100] = {};
        char mm[100] = {};
        strcpy(tmp, "");
        strcpy(hh, "");
        strcpy(mm, "");
        //display with mode(state)
        switch(state){
            case '0'://time
                OLED_Print(0x01, 0x02, 0x00, 0x7F, clock, sizeof(clock));
                Get_Timer_H(hh);
                Get_Timer_M(mm);
                OLED_PrintNumber(0x01, 0x02, 0x1D, 0x7F, hh);
                OLED_Print(0x01, 0x02, 0x44, 0x7F, dots, sizeof(dots));
                OLED_PrintNumber(0x01, 0x02, 0x59, 0x7F, mm);
                break;
            case'1'://temperature
                OLED_Print(0x01, 0x02, 0x00, 0x7F, thermometer, sizeof(thermometer));
                OLED_Print(0x01, 0x02, 0x70, 0x7F, temp, sizeof(temp));
                itoa(val.temp, tmp);
                OLED_PrintNumber(0x01, 0x02, 0x1C, 0x7F, tmp); 
                break;
            case'2'://steps
                OLED_Print(0x01, 0x02, 0x00, 0x7F, foots, sizeof(foots));
                itoa(val.step, tmp); 
                OLED_PrintNumber(0x01, 0x02, 0x1C, 0x7F, tmp);
                break;
            case'3'://how far
                itoa(val.step + val.step / 2, tmp);
                OLED_PrintNumber(0x01, 0x02, 0x1C, 0x7F, tmp);
                OLED_PrintChar(0x01, 0x02, 0x78, 0x7F, 'm');
                break;
            case'4'://heart rate
                if(which_heart == 1){
                    OLED_Print(0x01, 0x02, 0x00, 0x7F, heart1, sizeof(heart1));
                    which_heart = 2;
                }
                else{
                    OLED_Print(0x01, 0x02, 0x00, 0x7F, heart2, sizeof(heart2));
                    which_heart = 1;
                }
                //how many numbers
               // OLED_PrintNumber(0x01, 0x02, 0x1C, 0x7F, "123.45"); 
                itoa(val.heartRate, tmp);
                OLED_PrintNumber(0x01, 0x02, 0x1C, 0x7F, tmp);
                
                OLED_PrintChar(0x01, 0x02, 0x68, 0x7F, 'b');
                OLED_PrintChar(0x01, 0x02, 0x70, 0x7F, 'p');
                OLED_PrintChar(0x01, 0x02, 0x78, 0x7F, 'm');
                break;
        }

        __delay_ms(300);
    }

    return;
}
