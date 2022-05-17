
/*
 * led.h
 *
 *  Created on: May 16, 2022
 *      Author: user
 */

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

#ifndef LED_H_
#define LED_H_


void led_init();
void led_red();
void led_yellow();
void led_green();
void led_off();


#endif /* LED_H_ */
