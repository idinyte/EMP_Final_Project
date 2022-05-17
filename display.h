/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: display.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Test.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 220503  KA    Module created.
*
*****************************************************************************/

#ifndef _DISPLAY_H
  #define _DISPLAY_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

#include "led.h"

#define  EMPTY             -1
#define  INITIALIZE         0
#define  SELECT_COFFEE      1
#define  SELECT_PAYMENT     2
#define  CARD_PAYMENT       3
#define  CASH_PAYMENT       4
#define  ESPRESSO           9
#define  LATTE              10
#define  FILTER_COFFEE      11
#define  CASH               12
#define  CARD               13
#define  BREW               14

#define COFFEE_TYPES_LENGTH     5 // Allows for one digit types
#define COFFEE_CYCLE_TIME_MS    3000
#define COFFEE_NAME_LENGTH      14

#define SWITCH_POLL_DELAY_MS    500 // Also determines display flash rate
#define SLOW_DISPENSE_TIME_MS   2000.0f
#define SLOW_DISPENSE_AMOUNT    0.5f
#define FAST_DISPENSE_AMOUNT    1.0f
#define MAX_INACTIVITY_MS       5000.0f

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void display_task(void *pvParameters);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Tasks related to display
******************************************************************************/
INT8U check_input(INT8U *my_state, INT8U *coffee);
/*****************************************************************************
*   Input    : -
*   Output   : 0 if no input received and 1 if input received
*   Function : Checks keypad queue and decides what to display
******************************************************************************/

/****************************** End Of Module *******************************/

INT8U brew_coffee(INT8U *my_state, INT8U *coffee);

#endif

