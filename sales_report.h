/*
 * sales_report.h
 *
 *  Created on: May 11, 2022
 *      Author: bagas
 */

#include "logging.h"
#include "uart0.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#ifndef SALES_REPORT_H_
#define SALES_REPORT_H_

#define ASCII_ESC 27

typedef enum MENU{
    MAIN_MENU, LOG_LIST, SALES_REPORT
} MENU;

void ui_clear_screen();
void ui_task(void* pvParameters);
MENU main_menu();
MENU log_list_menu();
MENU sales_report_menu();

#endif /* SALES_REPORT_H_ */
