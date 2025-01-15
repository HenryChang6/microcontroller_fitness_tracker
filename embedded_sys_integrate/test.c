#include <stdio.h> 
#include "dataConfig.h"

void main(void) {
    printf("step num: %d\n", get_stepNum());
    printf("temperature: %2f\n", get_temperature());
    printf("beats: %2f\n", get_beats());
}