/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  RTOS.h
 *  Module:  Library/RTOS
 *  Description:  Scheduler Algorithm To Run My Real Time Operating System
*******************************************************************/
#ifndef _RTOS_H_
#define _RTOS_H_
/********************************************************************
*********************************************************************
          *---------------------------------------*
          *-------- MSP_S -> Top_Of_Stack --------*
          *---------------------------------------*
          *                                       * 
          *                                       * 
          *                                       * 
          *                                       * 
          *---------------------------------------*
          *--- MSP_E -> MSP_S-Main_Stack_Size ----*
          *---------------------------------------*
          *                                       *
          *----------- 32 Byte Safety ------------*
          *                                       *
          *---------------------------------------* 
          *- PSP_Base=Idle_Task=Start_PSP_Stack  -*
          *---------------------------------------* 
          *                                       * 
          *                                       * 
          *                                       * 
          *                                       * 
          *---------------------------------------*
          *--- Idle_Task + Idle_Task_Stack_Size --*
          *---------------------------------------*
          *                                       *
          *----------- 32 Byte Safety ------------*
          *                                       *
          *---------------------------------------* 
          *---------------- Tasks  ---------------*
          *---------------------------------------* 
          *                                       * 
          *                                       * 
          *                                       * 
          *                                       * 
          *                                       * 
          *                                       * 
          *                                       * 
          *                                       * 
          *                                       * 
          *---------------------------------------* 
          *----------  End -> __end_sp  ----------* 
          *---------------------------------------*
OS Will Running Over 8 MHZ Clock Internal Ossilator
     1- Freq = 8Mhz
     2- TicTime = 0.125us
     3- SchedulerTime = 1 ms
     4- Counts = 8000 Count
OS Has 4 States
     1-Suspend
     2-Wating
     3-Ready
     4-Running
OS Priority 0(Hi Periority) -> 255(Low Periority)
*********************************************************************
*********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "Macros.h"
/*****************************************
---------    Configure Macros    ---------
*****************************************/
/*Define The Maximum Number Of Tasks In The System*/
#define Maximum_Tasks                   (8U)
/*Define Main Stack Size(MSP)*/
#define Main_Stack_Size                 (1024U)
/*Comment This Line If You Wand Disable Hook Function*/
#define Enable_Context_Switch_Hook_Function
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
/*-------------- RTOS Task -------------*/
typedef struct RTOS_Task_t
{
     u8 Priority;
     u8 Name[19];
     u32 Stack_Size;
     void (*Task_Entry)(void);
}RTOS_Task_t;
/*-------------- RTOS Mutex ------------*/
typedef struct RTOS_Mutex_t
{
     u8 Name[16];
     void (*Owner_Entry_Function)(void);
     void (*Aquire_Entry_Function)(void);
}RTOS_Mutex_t;
/*****************************************
---  Application Programming Interface  --
*****************************************/
/****************************************************************************************************
* Function Name   : RTOS_Mutex_Aquire
* Description     : Attempts to acquire a mutex for the running task. If the mutex is already owned,
*                   the running task will be suspended until the mutex is released.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Assumed based on potential access to global data. Ensure restricted access in a multithreaded environment.)
* Parameters (in) : Mutex - Pointer to the mutex data structure.
* Parameters (out): None
* Return value    : Void
* Notes           : - The function first checks if the mutex has an owner. 
*                     If it doesn't, it sets the running task as the owner.
*                   - If the mutex already has an owner, it checks if an acquisition attempt has been made.
*                     If not, it sets the running task as the acquirer and suspends it.
*                   - If another task is already attempting to acquire the mutex, the function does nothing.
*                   - The function operates on the global `RTOS_Control` structure, which provides access to the running task.
*                   - Mutually exclusive access to shared resources is a core functionality in a real-time operating system (RTOS).
*                     This function facilitates that by ensuring that tasks do not access shared resources simultaneously.
*****************************************************************************************************/
void RTOS_Mutex_Aquire(RTOS_Mutex_t* Mutex);
/****************************************************************************************************
* Function Name   : RTOS_Mutex_Release
* Description     : Releases the mutex that may have been acquired by a task. If another task is 
*                   waiting to acquire this mutex, it is given ownership, and the previous owner 
*                   is preempted if necessary.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Assumes mutual exclusivity of access to Mutex. Ensure restricted access in a multithreaded environment.)
* Parameters (in) : Mutex - Pointer to the mutex to be released.
* Parameters (out): None
* Return value    : Void
* Notes           : - If a task is waiting to acquire the mutex (`Aquire_Entry_Function` is not NULL), 
*                     and the current owner is the running task, then ownership is transferred to the waiting task.
*                   - The previous owner's and the new owner's states are updated to 'Waiting' to possibly 
*                     trigger a context switch on the next scheduler run.
*                   - `RTOS_Service` is invoked with `RTOS_Service_Activate_Task` to ensure that the 
*                     scheduler considers the updated task states.
*                   - If no task is waiting to acquire the mutex, or if the running task is not the current 
*                     owner, the function simply returns without any action.
*                   - It's important to ensure that mutexes are always released by the task that acquired them.
*                   - This function uses `Get_Task_TCB` to get the TCB of the new mutex owner.
*****************************************************************************************************/
void RTOS_Mutex_Release(RTOS_Mutex_t* Mutex);
/********************************************************************
* Syntax          : static void RTOS_Initialize_Stack(void)
* Description     : Initialize the Main Stack Pointer (MSP) and Process Stack Pointer (PSP) for tasks.
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
* Notes           : - Initializes the starting point of the MSP using the system's initial stack pointer.
*                   - Determines the end of the MSP based on the main stack size.
*                   - Initializes the PSP for tasks by setting it just before the end of the MSP, considering alignment.
********************************************************************/
void RTOS_Initialize(void);
/********************************************************************
* Syntax          : void RTOS_Run_Tasks(void)
* Description     : Starts the RTOS scheduler and initiates execution with the idle task.
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
* Notes           : This function is used to start the OS by running all created tasks 
                    and starting the first tick with the idle task.
********************************************************************/
void RTOS_Run_Tasks(void);
/********************************************************************
* Syntax          : RTOS_Create_Task(RTOS_Task_t Task)
* Description     : Create a new task and initialize its related data and stack.
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Task - Structure containing task details like priority, name, stack size, and task entry function.
* Parameters (out): None
* Return value    : Void
* Notes           : - Stores the task data if there is available stack space.
*                   - Defines the start point of the task's stack.
*                   - Updates the PSP (Process Stack Pointer) to allocate space for the new task and aligns it.
*                   - Initializes the stack space for this task.
*                   - Sets the state of the task to "Suspend" initially.
*                   - Configures the task's waiting state as "Not Waiting".
*                   - Increments the task counter to keep track of total tasks.
********************************************************************/
void RTOS_Create_Task(RTOS_Task_t Task);
/****************************************************************************************************
* Function Name   : RTOS_Activate_Task
* Description     : Activates a specified task and brings it into the Ready state.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Assumes mutual exclusivity when changing the state of tasks. Ensure restricted access in a multithreaded environment.)
* Parameters (in) : Task - Specifies the task to be activated.
* Parameters (out): None
* Return value    : Void
* Notes           : - The current running task's state is set to 'Waiting'.
*                   - Calls the kernel-specific `RTOS_Kernal_Activate_Task` to handle low-level task activation details.
*                   - Uses `Get_Task_TCB` to retrieve the TCB of the task to be activated.
*****************************************************************************************************/
void RTOS_Activate_Task(RTOS_Task_t Task);
/****************************************************************************************************
* Function Name   : RTOS_Terminate_Task
* Description     : Terminates a specified task and removes it from the Ready or Running state.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Assumes mutual exclusivity when changing the state of tasks. Ensure restricted access in a multithreaded environment.)
* Parameters (in) : Task - Specifies the task to be terminated.
* Parameters (out): None
* Return value    : Void
* Notes           : - The current running task's state is set to 'Waiting'.
*                   - Calls the kernel-specific `RTOS_Kernal_Terminate_Task` to handle low-level task termination details.
*                   - Uses `Get_Task_TCB` to retrieve the TCB of the task to be terminated.
*                   - It's important to ensure that tasks are terminated properly to avoid resource leaks or system instability.
*****************************************************************************************************/
void RTOS_Terminate_Task(RTOS_Task_t Task);
/****************************************************************************************************
* Function Name   : RTOS_Delay
* Description     : Delays the execution of the current running task for a specified number of ticks.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Assumes mutual exclusivity when modifying task states and waiting times. Ensure restricted access in a multithreaded environment.)
* Parameters (in) : Ticks_Count - Specifies the delay duration in ticks.
* Parameters (out): None
* Return value    : Void
* Notes           : - The state of the current running task is set to 'Suspend' during the delay period.
*                   - The task's waiting time is configured to the specified `Ticks_Count`.
*                   - After setting the delay, the current task is terminated using the `RTOS_Service_Terminate_Task` function to allow other tasks to execute during the delay.
*                   - The task will be resumed once the waiting time is elapsed.
*                   - It's essential to avoid using long delays that might adversely affect real-time performance.
*****************************************************************************************************/
void RTOS_Delay(u32 Ticks_Count);
/********************************************************************
* Syntax          : void RTOS_Idle_Task(void (*Idle_Task)(void))
* Description     : Assigns the idle task callback function.
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Idle_Task - Pointer to the function that should be executed during the idle state.
* Parameters (out): None
* Return value    : Void
* Notes           : - This function allows customization of the idle task behavior.
*                   - The provided function will be executed whenever the system is idle.
********************************************************************/
void RTOS_Idle_Task(void (*Idle_Task)(void));
#ifdef Enable_Context_Switch_Hook_Function
	void RTOS_Context_Switch_Hook_Function(void (*Hook_Function)(void));
#endif
/********************************************************************
 *  END OF FILE:  RTOS.h
********************************************************************/
#endif
