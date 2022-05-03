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
#include "lcd.h"
#include "glob_def.h"
#include "tmodel.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "string.h"
#include "display.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
extern SemaphoreHandle_t drink_selection;
extern QueueHandle_t key_queue;
extern SemaphoreHandle_t lcd_mutex;
/*****************************   Functions   *******************************/

void display_task(void *pvParameters)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function :
 ******************************************************************************/
{
    static INT8U my_state = INITIALIZE;
    static INT8U coffee = ESPRESSO;
    INT8U price;
    INT8U key_value;
    while (1)
    {
        switch (my_state)
        {
        case INITIALIZE:
            price = 18;
            gfprintf( COM2, "%c%c%c4%c%c%02d DKK%c%c6%c", 0x1B, 0x80, 0x7F,
                     0x1B, 0x85, price, 0x1B, 0x8E, 0x7E);
            move_LCD(4, 1);
            gfprintf( COM2, "espresso");
            my_state = SELECT_COFFEE;
            break;
        case SELECT_COFFEE:
            if (xSemaphoreTake(lcd_mutex, 0)) // if lcd_queue empty
            {
            if (uxSemaphoreGetCount(drink_selection)) // button 4 or 6 pushes detected
            {
                while (xSemaphoreTake(drink_selection, 0)) // loop until we are at the latest button push
                {
                    xQueueReceive(key_queue, &key_value, 0);
                    while ((INT8U) key_value != '4' && (INT8U) key_value != '6') // skip button pushes in button queue that are not 4 or 6
                    {
                        xQueueReceive(key_queue, &key_value, 0);
                        if (uxQueueMessagesWaiting(key_queue) == 0) // return if key_queue empty
                        {
                            break;
                        }
                    }
                    if ((INT8U) key_value == '4') // go back if pushed key is 4
                    {
                        coffee -= 1;
                    }
                    else if ((INT8U) key_value == '6') // go forward if pushed key is 6
                    {
                        coffee += 1;
                    }
                }
                switch (coffee % 3) // decide which screen to display
                {
                case 0:
                    coffee = ESPRESSO;
                    break;
                case 1:
                    coffee = LATTE;
                    break;
                case 2:
                    coffee = FILTER_COFFEE;
                    break;

                }
                switch (coffee) // display coffee select screen on LCD
                {
                case ESPRESSO:
                    price = 18;
                    // clear screen, escape, set cursor, left arrow symbol, escape, set cursor, price, escape, set cursor, right arrow symbol
                    gfprintf( COM2, "%c%c%c%c4%c%c%02d DKK%c%c6%c", 0xFF, 0x1B, 0x80,
                             0x7F, 0x1B, 0x85, price, 0x1B, 0x8E, 0x7E);
                    move_LCD(4, 1);
                    gfprintf( COM2, "espresso");
                    break;
                case LATTE:
                    price = 26;
                    gfprintf( COM2, "%c%c%c%c4%c%c%02d DKK%c%c6%c", 0xFF, 0x1B, 0x80,
                             0x7F, 0x1B, 0x85, price, 0x1B, 0x8E, 0x7E);
                    move_LCD(5, 1);
                    gfprintf( COM2, "latte");
                    break;
                case FILTER_COFFEE:
                    price = 2;
                    gfprintf( COM2, "%c%c%c%c4%c%c%d DKK/cl%c%c6%c", 0xFF, 0x1B, 0x80,
                             0x7F, 0x1B, 0x84, price, 0x1B, 0x8E, 0x7E);
                    move_LCD(2, 1);
                    gfprintf( COM2, "filter coffee");
                    break;
                }
            }
            xSemaphoreGive(lcd_mutex);
            }
            break;
        }
    }

}
/****************************** End Of Module *******************************/

