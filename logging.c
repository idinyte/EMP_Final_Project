/*
 * logging.c
 *
 *  Created on: May 10, 2022
 *      Author: bagas
 */

#include "logging.h"

QueueHandle_t log_coffee_queue;
QueueHandle_t log_payment_queue;
LOG_TYPE log_array[LOG_LENGTH];
SemaphoreHandle_t log_array_semaphore;

extern COFFEE_TYPE coffee_type[COFFEE_TYPES_LENGTH];
extern SemaphoreHandle_t coffee_type_mutex;

void log_init(){
    //create queue for logging
    log_coffee_queue = xQueueCreate(LOG_INPUT_LENGTH, LOG_COFFEE_WIDTH);
    configASSERT(log_coffee_queue);
    log_payment_queue = xQueueCreate(LOG_INPUT_LENGTH, LOG_PAYMENT_WIDTH);
    configASSERT(log_payment_queue);

    //semaphore for array
    log_array_semaphore = xSemaphoreCreateMutex();
    xSemaphoreGive(log_array_semaphore);
}

void log_task(void* pvParameters){
    COFFEE_TYPE coffee_type;
    INT8U payment_type[CARD_LENGTH];

    while(1){
        //queue receive
        xQueueReceive(log_coffee_queue, &coffee_type, portMAX_DELAY);
        xQueueReceive(log_payment_queue, &payment_type, portMAX_DELAY);

        //semaphore take
        xSemaphoreTake(log_array_semaphore, portMAX_DELAY);

        LOG_TYPE* log = log_nextlog();
        log->active = 1;
        log->price = coffee_type.price;

        //convert payment type to string
        if(payment_type[0] == CASH){
           strcpy(&log->payment_type,"CASH");
        }
        else{
            for(INT8U i = 0; i < CARD_LENGTH; i++){
                log->payment_type[i] = payment_type[i] + '0';
            }
        }

        //get coffee type
        xSemaphoreTake(coffee_type_mutex, portMAX_DELAY);
        int num = -1;

        for(INT8U i = 0, i < COFFEE_TYPES_LENGTH; i++){
            if(!strcmp(coffee_type[i].name, coffee_type.name)){
                num = i;
                break;
            }
        }

        xSemaphoreGive(coffee_types_mutex);
        configASSERT(num != -1);

        log->coffee_number = num;

        xSemaphoreGive(log_array_semaphore);
    }
}

void log_coffee(COFFEE_TYPE* coffee){
    xQueueSendToBack(log_coffee_queue, coffee, portMAX_DELAY);
}

void log_payment(INT8U* payment_type){
    xQueueSendToBack(log_payment_queue, payment_type, portMAX_DELAY);
}

LOG_TYPE* log_nextLog(){
    for(int i = 0; i < LOG_LENGTH; i++){
        if(!log_array[i].active){
            return &log_array[i];
        }
    }
    return NULL;
}


