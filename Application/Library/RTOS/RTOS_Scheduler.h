/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  RTOS_Scheduler.h
 *  Module:  Library/RTOS
 *  Description:  Scheduler Algorithm To Run My Real Time Operating System
*******************************************************************/
#ifndef _RTOS_SCHEDULER_H_
#define _RTOS_SCHEDULER_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "RTOS.h"
/*****************************************
---------    Configure Macros    ---------
*****************************************/
#define Idle_Task_Stack_Size            (256U)
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
/*------------- RTOS States ------------*/
typedef enum RTOS_State_t
{
     RTOS_State_Suspend                  =(0U),
     RTOS_State_Running                  =(1U),
     RTOS_State_Waiting                  =(2U),
     RTOS_State_Ready                    =(3U)
}RTOS_State_t;
/*------------- RTOS States ------------*/
typedef enum RTOS_Service_t
{
     RTOS_Service_Activate_Task         =(0U),
     RTOS_Service_Terminate_Task        =(1U),
     RTOS_Service_Waiting_For_Task      =(2U),
}RTOS_Service_t;
/*---------- RTOS Waiting Time ---------*/
typedef struct __attribute__((packed)) RTOS_Waiting_Time_t
{
     enum{Not_Waiting,Waiting}State;
     u32 Ticks_Number;
}RTOS_Waiting_Time_t;
/*--------- TCB Conistruction -----------*/
typedef struct __attribute__((packed)) RTOS_TCB_t
{
    u32* Current_Task_PSP;
    u32 Start_PSP_Stack;
    RTOS_Task_t Task;     
    RTOS_State_t State;
    RTOS_Waiting_Time_t Waiting_Time; 
}RTOS_TCB_t;
/*--------- TCB Conistruction -----------*/
typedef enum RTOS_Mode_t
{
     RTOS_Suspend                  =(0U),
     RTOS_Running                  =(1U)
}RTOS_Mode_t;
/*--------- TCB Conistruction -----------*/
typedef struct RTOS_Control_t
{
     RTOS_TCB_t Tasks[Maximum_Tasks];
     RTOS_TCB_t* Running_Task;
     RTOS_TCB_t* Pending_Task;
     u32* Current_PSP;
     u32 Start_MSP_Stack;
     u32 End_MSP_Stack;
     u8 Tasks_Count;
     u8 Padding[2];
     RTOS_Mode_t Mode;
}RTOS_Control_t;
/*****************************************
----------    Defined Macros    ----------
*****************************************/
#define Activate_Task                   0x00
#define Terminate_Task                  0x01
#define Waiting_Task                    0x02
/*****************************************
---  Application Programming Interface  --
*****************************************/
/****************************************************************************************************
* Function Name   : RTOS_Update_Scheduler_Table
* Description     : Updates the RTOS scheduler's table, sorting tasks by priority and updating the ready queue.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Assumed based on manipulation of global data. Ensure restricted access in a multithreaded environment.)
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
* Notes           : - Initially, tasks are sorted based on their priority using the bubble sort algorithm.
*                   - The ready queue is then emptied to ensure fresh and accurate scheduling.
*                   - The function loops through tasks, checking their states and priorities.
*                   - If a task is not suspended, it's evaluated to see its place in the ready queue.
*                   - Tasks are enqueued in the ready queue based on their priority relative to other tasks.
*                   - The function works with the global `RTOS_Control` structure which holds information about tasks and their states.
*****************************************************************************************************/
void RTOS_Update_Scheduler_Table(void);
/****************************************************************************************************
* Function Name   : RTOS_Service
* Description     : Routes the requested service to the appropriate supervisor call based on the provided service ID.
* Sync-Async      : Synchronous
* Reentrancy      : Reentrant (Assumed based on code structure. Validate depending on the Supervisor_Call implementation.)
* Parameters (in) : Service_ID - ID of the requested service.
* Parameters (out): None
* Return value    : Void
* Notes           : - The function acts as a dispatcher for different RTOS services.
*                   - Depending on the provided Service_ID, a corresponding supervisor call is made.
*                   - Supported services include activating tasks, terminating tasks, and putting tasks in waiting state.
*****************************************************************************************************/
void RTOS_Service(RTOS_Service_t Service_ID);
/****************************************************************************************************
* Function Name   : RTOS_Kernal_Activate_Task
* Description     : Activates a given task by updating its state to "Waiting" and subsequently updating the scheduler table.
* Sync-Async      : Synchronous
* Reentrancy      : Reentrant (Assumed based on code structure. Validate if true based on the overall system.)
* Parameters (in) : Task_TCB - Pointer to the Task Control Block (TCB) of the task to be activated.
* Parameters (out): None
* Return value    : Void
* Notes           : 
*                   - The function directly changes the state of the specified task to "Waiting".
*                   - A service request to activate the task is then made to update the scheduler table.
*****************************************************************************************************/
void RTOS_Kernal_Activate_Task(RTOS_TCB_t* Task_TCB);
/****************************************************************************************************
* Function Name   : RTOS_Kernal_Terminate_Task
* Description     : Terminates a given task by updating its state to "Suspend" and then updates the scheduler table.
* Sync-Async      : Synchronous
* Reentrancy      : Reentrant (Assumed based on code structure. Validate if true based on the overall system.)
* Parameters (in) : Task_TCB - Pointer to the Task Control Block (TCB) of the task to be terminated.
* Parameters (out): None
* Return value    : Void
* Notes           : 
*                   - The function directly changes the state of the specified task to "Suspend".
*                   - A service request to terminate the task is then made to update the scheduler table.
*****************************************************************************************************/
void RTOS_Kernal_Terminate_Task(RTOS_TCB_t* Task_TCB);
/********************************************************************
 *  END OF FILE:  RTOS_Scheduler.h
********************************************************************/
#endif
