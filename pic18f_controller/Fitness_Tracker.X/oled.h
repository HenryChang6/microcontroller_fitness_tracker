#ifndef OLED_H
#define	OLED_H

#define _OLED_ADDR 0x3C
#include <stdint.h> 
void I2C_Start();
void I2C_Wait();
void I2C_Stop();
void I2C_Write(uint8_t data);
void OLED_SetPageAndColumnAddress(const uint8_t startPage, const uint8_t endPage, const uint8_t startColumn, const uint8_t endColumn);
void OLED_Clear();
void OLED_Print(const uint8_t startPage, const uint8_t endPage, const uint8_t startColumn, const uint8_t endColumn, const uint8_t *bitmap, uint16_t bitmapSize);
void OLED_PrintChar(const uint8_t startPage, const uint8_t endPage, const uint8_t startColumn, const uint8_t endColumn, char c);
void OLED_PrintNumber(const uint8_t startPage, const uint8_t endPage, const uint8_t startColumn, const uint8_t endColumn, const char* str);

#endif