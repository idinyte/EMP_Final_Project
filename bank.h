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
INT8U card_payment(INT8U last_card, INT8U last_pin, INT8U verify);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : verify card payment
******************************************************************************/


/****************************** End Of Module *******************************/
#endif

