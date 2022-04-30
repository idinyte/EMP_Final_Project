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

#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define IDLE_PRIO 0
#define LOW_PRIO  1
#define MED_PRIO  2
#define HIGH_PRIO 3

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


    // creating tasks
    xTaskCreate( status_led_task, "Status_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL );

    // starting scheduler
    vTaskStartScheduler();

    return 0;
}
