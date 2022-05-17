/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: leds.c
 *
 * PROJECT....: ECP
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 050128  KA    Module created.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "bank.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
/*****************************   Functions   *******************************/

INT8U card_payment(INT8U last_card, INT8U last_pin, INT8U verify)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : verify if card number and pin are both even or odd
 ******************************************************************************/
{
    static INT8U last_card_digit;
    if (verify)
    {
        return (last_card_digit & 1) == (last_pin & 1);
    }
    else
    {
        last_card_digit = last_card;
        return 0;
    }
}


/****************************** End Of Module *******************************/

