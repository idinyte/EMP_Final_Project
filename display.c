/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: display.c
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
 * 220504  KA    Module created.
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
//extern QueueHandle_t coffee_queue;
extern SemaphoreHandle_t lcd_mutex;
/*****************************   Functions   *******************************/

void display_task(void *pvParameters)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : send what to print to lcd_queue
 ******************************************************************************/
{
    static INT8U my_state = INITIALIZE;
    static INT8U screen = ESPRESSO;
    static INT8U counter = 0;
    INT8U price;
    INT8U card_ch;
    BOOLEAN amount_pay;
    FP32 grind_time;
    FP32 brew_time;
    FP32 milk_time;
    FP32 inactivity;
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
            grind_time = 5.0f;
            brew_time = 15.0f;
            milk_time = 0.0f;
            my_state = SELECT_COFFEE;
            break;
        case SELECT_COFFEE:
            if (check_input(&my_state, &screen)) // if input received
            {
                switch (screen)
                {
                case ESPRESSO:
                    price = 18;
                    // clear screen, escape, set cursor, left arrow symbol, escape, set cursor, price, escape, set cursor, right arrow symbol
                    gfprintf( COM2, "%c%c%c%c4%c%c%02d DKK%c%c6%c", 0xFF, 0x1B,
                             0x80, 0x7F, 0x1B, 0x85, price, 0x1B, 0x8E, 0x7E);
                    move_LCD(4, 1);
                    gfprintf( COM2, "espresso");
                    grind_time = 5.0f;
                    brew_time = 15.0f;
                    milk_time = 0.0f;
                    break;
                case LATTE:
                    price = 26;
                    gfprintf( COM2, "%c%c%c%c4%c%c%02d DKK%c%c6%c", 0xFF, 0x1B,
                             0x80, 0x7F, 0x1B, 0x85, price, 0x1B, 0x8E, 0x7E);
                    move_LCD(5, 1);
                    gfprintf( COM2, "latte");
                    grind_time = 5.0f;
                    brew_time = 15.0f;
                    milk_time = 3.0f;
                    break;
                case FILTER_COFFEE:
                    price = 2;
                    gfprintf( COM2, "%c%c%c%c4%c%c%d DKK/cl%c%c6%c", 0xFF, 0x1B,
                             0x80, 0x7F, 0x1B, 0x84, price, 0x1B, 0x8E, 0x7E);
                    move_LCD(2, 1);
                    gfprintf( COM2, "filter coffee");
                    grind_time = 0.0f;
                    brew_time = 0.0f;
                    milk_time = 0.0f;
                    break;
                }
                my_state = BREW;
            }
            break;
        case SELECT_PAYMENT:
            if (check_input(&my_state, &screen)) // if input received
            {
                switch (screen)
                {
                case CASH:
                    // clear screen, escape, set cursor, left arrow symbol, escape, set cursor, price, escape, set cursor, right arrow symbol
                    gfprintf( COM2, "%c%c%c%c4%c%cPay with%c%c6%c", 0xFF, 0x1B,
                             0x80, 0x7F, 0x1B, 0x84, 0x1B, 0x8E, 0x7E);
                    move_LCD(6, 1);
                    gfprintf( COM2, "cash");
                    break;
                case CARD:
                    gfprintf( COM2, "%c%c%c%c4%c%cPay with%c%c6%c", 0xFF, 0x1B,
                             0x80, 0x7F, 0x1B,  0x84, 0x1B, 0x8E, 0x7E);
                    move_LCD(6, 1);
                    gfprintf( COM2, "card");
                    break;
                }
            }
            break;
        case CARD_PAYMENT:
            if (xQueueReceive(key_queue, &card_ch, 0)) // if input received
            {
                if (card_ch == '#' || card_ch == '*')
                {
                    break;
                }
                counter += 1;
                if (xSemaphoreTake(lcd_mutex, portMAX_DELAY))
                {
                    if (counter < 16) // if card number
                    {
                        wr_ch_LCD(card_ch);
                    }
                    else if (counter == 16)
                    {
                        wr_ch_LCD(card_ch);
                        gfprintf( COM2, "%cEnter card pin", 0xFF);
                        move_LCD(0, 1);
                        card_payment(card_ch, 0, 0); // send last card number to a banking function
                    }
                    else if (counter == 20)
                    {
                        wr_ch_LCD('*');
                        if (card_payment(0, card_ch, 1)) // verify payment
                        {
                            gfprintf( COM2, "%cPayment accepted", 0xFF);
                            vTaskDelay(1800 / portTICK_RATE_MS);
                        }
                        else
                        {
                            gfprintf( COM2, "%cPayment declined", 0xFF);
                            vTaskDelay(1800 / portTICK_RATE_MS);
                            gfprintf( COM2, "%cEnter card No.", 0xFF);
                            move_LCD(0, 1);
                            counter = 0;
                        }
                    }
                    else // if pin
                    {
                        wr_ch_LCD('*');
                    }
                    xSemaphoreGive(lcd_mutex);
                }
            }
            break;
        case BREW:
            inactivity = 0.0f;
            BOOLEAN choosed_coffee = (screen == ESPRESSO || screen == FILTER_COFFEE || screen == LATTE);
            while(1)
            {
                xSemaphoreTake(lcd_mutex, portMAX_DELAY);
                if(inactivity > MAX_INACTIVITY_MS)
                {
                    break;
                }
                else if(!get_sw2()) // if true skip
                {
                    if(price != 0 && choosed_coffee)
                    {
                        gfprintf( COM2, "\n Place Cup");
                        inactivity += 100.0f;
                    }
                }
                else if(!get_sw1()) // if true skip
                {
                    if(price != 0 && choosed_coffee)
                    {
                        gfprintf( COM2, "\n Hold Start");
                        inactivity += 100.0f;
                    }
                }
                else{
                    inactivity = 0.0f;


                    if (grind_time > 0.0f)
                    {
                        led_red();
                        gfprintf(0, "Grinding...");
                        // still need to find time in second
                        grind_time -= 100.0f / 1000.0f;
                    }
                    else if (brew_time > 0.0f)
                    {
                        led_yellow();
                        gfprintf(0, "Brewing...");
                        brew_time -= 100.0f / 1000.0f;
                    }
                    else if (milk_time > 0.0f)
                    {
                        led_green();
                        gfprintf(0, "Milk froth...");
                        milk_time -= 100.0f / 1000.0f;
                    }
                    else
                    {
                        my_state = INITIALIZE;
                        break;
                    }
                }
            }
            break;
        }
    }
}

INT8U check_input(INT8U *my_state, INT8U *screen)
{
    INT8U key_value;
    INT8U input_received = 0;
    if (xSemaphoreTake(lcd_mutex, 0))
    {
        if (uxSemaphoreGetCount(drink_selection)) // valid input detected
        {
            input_received = 1;
            while (xSemaphoreTake(drink_selection, 0)) // loop until we are at the latest button push
            {
                xQueueReceive(key_queue, &key_value, 0);
                // skip button pushes that don't do anything
                while (key_value != '4' && key_value != '6' && key_value != '5'
                        && key_value != '#' && key_value != '*')
                {
                    xQueueReceive(key_queue, &key_value, 0);
                    if (uxQueueMessagesWaiting(key_queue) == 0) // return if key_queue empty
                    {
                        break;
                    }
                }
                if (key_value == '4') // go back if pushed key is 4
                {
                    *screen -= 1;
                }
                else if (key_value == '6') // go forward if pushed key is 6
                {
                    *screen += 1;
                }
                else if (key_value == '5' || key_value == '#'
                        || key_value == '*') // select
                {
                    if (*my_state == SELECT_COFFEE)
                    {
                        gfprintf( COM2, "%c%c%c%c4%c%cPay with%c%c6%c", 0xFF,
                                 0x1B, 0x80, 0x7F, 0x1B, 0x84, 0x1B, 0x8E,
                                 0x7E);
                        move_LCD(6, 1);
                        gfprintf( COM2, "cash");
                        *my_state = SELECT_PAYMENT;
                        *screen = CASH;
                    }
                    else if (*my_state == SELECT_PAYMENT && *screen == CARD)
                    {
                        xQueueReset(key_queue);
                        gfprintf( COM2, "%cEnter card No.", 0xFF);
                        move_LCD(0, 1);
                        *my_state = CARD_PAYMENT;
                        *screen = EMPTY; // not empty, just genererated in display_task
                    }
                }
                // decide which screen to display
                if (*my_state == SELECT_COFFEE)
                {
                    switch (*screen % 3)
                    {
                    case 0:
                        *screen = ESPRESSO;
                        break;
                    case 1:
                        *screen = LATTE;
                        break;
                    case 2:
                        *screen = FILTER_COFFEE;
                        break;
                    }
                }
                else if (*my_state == SELECT_PAYMENT)
                {
                    switch (*screen % 2)
                    {
                    case 0:
                        *screen = CASH;
                        break;
                    case 1:
                        *screen = CARD;
                        break;
                    }
                }

            }
        }
        xSemaphoreGive(lcd_mutex);
    }
    return input_received;
}

//INT8U brew_coffee(INT8U *my_state, INT8U *coffee){
//
//}


/****************************** End Of Module *******************************/

