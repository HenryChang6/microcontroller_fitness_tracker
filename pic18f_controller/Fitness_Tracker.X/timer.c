#include <xc.h>
#include "timer.h"
#include <string.h>

int cnt = 0;

void update_time() {
    cnt++;
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