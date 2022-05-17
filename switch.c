/*
 * switch.c
 *
 *  Created on: May 11, 2022
 *      Author: bagas
 */

#include "switch.h"

void switch_init(){
    //enable port F
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;

    //SW2 enable
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;

    //enable pins
    GPIO_PORTF_DEN_R |= 0x11;
    //pull up input
    GPIO_PORTF_PUR_R |= 0x11;
}

INT8U get_sw1(){
    return !(GPIO_PORTF_DATA_R & 0x01);
}
INT8U get_sw2(){
    return !(GPIO_PORTF_DATA_R & 0x10);
}

