/**
 * main.c
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick_frt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "status_led.h"
#include "adc.h"
#include "gpio.h"
#include "lcd.h"
#include "tmodel.h"
#include "queue.h"

#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE // 70
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

QueueHandle_t lcd_queue; // used for accessing the pushing to queue

static void setupHardware(void)
/*****************************************************************************
*   Input    :  -
*   Output   :  -
*   Function :
*****************************************************************************/
{

  init_gpio();
  // Warning: If you do not initialize the hardware clock, the timings will be inaccurate
  init_systick();
  status_led_init();
  init_adc();
}

int main(void)
{
    // initialization
    setupHardware();

    // opening queues
    lcd_queue = xQueueCreate(LCD_QUEUE_LEN, sizeof(INT8U)); // (queue size, size of element in the queue)

    // creating tasks
    xTaskCreate( status_led_task, "Status_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );
    xTaskCreate( lcd_task, "LCD", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

    // starting scheduler
    vTaskStartScheduler();

    return 0;
}
