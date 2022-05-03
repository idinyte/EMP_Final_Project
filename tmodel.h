/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: tmodel.h
*
* PROJECT....: ECP
*
* DESCRIPTION: Defines the elemtn of the task model..
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 101004  MoH   Module created.
*
*****************************************************************************/

#ifndef _TMODEL_H_
#define _TMODEL_H_

/* not used so far
// Tasks.
// ------
#define TASK_LCD         USER_TASK


// Semaphores.
// -----------
#define SEM_LCD          USER_SEM
#define SEM_RTC_UPDATED  USER_SEM+1
*/
#define SEM_LEN_MED    32

// QUEUEs.
// -------
#define LCD_QUEUE_LEN 128
#define KEY_QUEUE_LEN 64


#endif /* _TMODEL_H_ */
