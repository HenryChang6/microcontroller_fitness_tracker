#include <xc.h>
#include "timer.h"
#include <string.h>

int cnt = 0;
void reverse(char *first, char *last) {
    for (;first < last; first++,last--) {
        // swap
        char tmp = *first;
        *first = *last;
        *last = tmp;
    }
}
void init_time() {
    cnt = 0;
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