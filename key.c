/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: key.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150321  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "tmodel.h"
#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t key_queue;

INT8U row( INT8U y )
{
  INT8U result = 0;

  switch( y )
  {
    case 0x01: result = 1; break;
    case 0x02: result = 2; break;
    case 0x04: result = 3; break;
    case 0x08: result = 4; break;
  }
  return( result );
}

INT8U key_catch( x, y )
INT8U x, y;
{
  const INT8U matrix[3][4] = {{'*','7','4','1'},
                              {'0','8','5','2'},
                              {'#','9','6','3'}};

  return( matrix[x-1][y-1] );
}

BOOLEAN get_keyboard( INT8U *pch )
{
  return( xQueueReceive(key_queue, &pch, 0) );
}

BOOLEAN check_column(INT8U x)
{
    INT8U y = GPIO_PORTE_DATA_R & 0x0F;             // Save the values of the 4 bits for the rows
    if( y )                                         // If one of them are set...
    {                                               // ...we first find the row number with the function row()
        INT8U ch = key_catch( x, row(y) );          // Now that we have the row and column we look up the corresponding character using the function key_catch
        xQueueSend( key_queue, &ch, 1 );            // Put the character in a queue so it can be used by another task
        return 1;
    }
    return 0;
}

extern void key_task(void *pvParameters)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function :
 ******************************************************************************/
{
    static INT8U my_state = 0;
    while (1)
    {
        switch (my_state)
        {
        case 0:
            GPIO_PORTA_DATA_R &= 0xE3;       // Clear the 3 bits for the columns
            GPIO_PORTA_DATA_R |= 0x10;          // Set the bit for column 1
            if (check_column(1)) // Check all the rows for column 1, using the function check_column
            { // If a button press is registered we go to next state so the press is only registered once
                my_state = 1;
                break;
            }
            GPIO_PORTA_DATA_R &= 0xE3; // Repeat the above for the two other columns
            GPIO_PORTA_DATA_R |= 0x08;
            if (check_column(2))
            {
                my_state = 1;
                break;
            }
            GPIO_PORTA_DATA_R &= 0xE3;
            GPIO_PORTA_DATA_R |= 0x04;
            if (check_column(3))
            {
                my_state = 1;
                break;
            }
            break;
        case 1:
            if (!(GPIO_PORTE_DATA_R & 0x0F)) // We stay here until the button is released so a button press is not counted more than once
            {
                vTaskDelay(40 / portTICK_RATE_MS);
                if (!(GPIO_PORTE_DATA_R & 0x0F)) // second layer of protection to be sure that the button is still released after 40 ms
                {
                    my_state = 0;
                }
            }
            break;
        }
    }

}
