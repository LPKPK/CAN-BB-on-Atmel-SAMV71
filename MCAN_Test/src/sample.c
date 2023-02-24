#include "stdio.h"

static uint16_t count_1=0;
static uint16_t count_2=0;

void sample_init();
void sample_1(){

    count_1++;
 
};

uint8_t add_1(){
    return count_1;
}