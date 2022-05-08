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
#define  INITIALIZE         0
#define  SELECT_COFFEE      1
#define  SELECT_PAYMENT     2
#define  ESPRESSO           9
#define  LATTE              10
#define  FILTER_COFFEE      11
#define  CASH               12
#define  CARD               13
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
#endif

