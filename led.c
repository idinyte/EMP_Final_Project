/*
 * led.c
 *
 *  Created on: May 11, 2022
 *      Author: bagas
 */

#include "led.h"

void led_init(){
    //enable port F
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCPIO_R5;

    //enable pins
    GPIO_PORTF_DEN_R |= 0x0E;
    //set as output
    GPIO_PORTF_DIR_R |= 0x0E;
}

void led_red(){
    GPIO_PORTF_DATA_R &= ~(0x02);
}
void led_yellow(){
    GPIO_PORTF_DATA_R &= ~(0x04);
}
void led_green(){
    GPIO_PORTF_DATA_R &= ~(0x08);
}
void led_off(){
    GPIO_PORTF_DATA_R |= (0x0E);
}


