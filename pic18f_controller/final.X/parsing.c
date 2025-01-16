/*
 * File:   parsing.c
 * Author: jyunanchen
 *
 * Created on January 16, 2025, 3:48 AM
 */
#include <xc.h>
#include "parsing.h"
#include "uart.h"
#include <string.h>
struct Data {
    int step;
    int heartRate;
    int temp;
};

struct Data ParsingData(char* s) {
    int num1 = -1, num2 = -1, num3 = -1;
    char *token;
    char input[100];
    strcpy(input, s);
    ClearBuffer();
    token = strtok(input, " ");
    num1 = atoi(token);

    token = strtok(NULL, " ");
    num2 = atoi(token);

    token = strtok(NULL, " ");
    num3 = atoi(token);
    
    struct Data val;
    val.step = num1;
    val.heartRate = num2;
    val.temp = num3;
    
    return val;
}