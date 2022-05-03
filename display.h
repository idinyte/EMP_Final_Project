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
#define  PAYMENT_METHOD     2
#define  ESPRESSO           9
#define  LATTE              10
#define  FILTER_COFFEE      11
/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void display_task(void *pvParameters);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Tasks related to display
******************************************************************************/


/****************************** End Of Module *******************************/
#endif

