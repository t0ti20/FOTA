/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  RTOS.c
 *  Module:  Library/RTOS
 *  Description:  Scheduler Algorithm To Run My Real Time Operating System
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "RTOS_Scheduler.h"
#include "RTOS_Porting.h"
#include "Queue.h"
#include "RTOS.h"
/*****************************************
--------    GLOBAL Variables    ----------
*****************************************/
/*Data Base That Contain All Tasks*/
extern RTOS_Control_t RTOS_Control;
RTOS_Control_t RTOS_Control;
/*Ready Queue To Store PTR To TCB Of Ready To Run Tasks*/
extern Queue_t Ready_Queue;
Queue_t Ready_Queue;
/*Idle Task Call Back Pointer*/
static void (*Idle_Task_CallBack)(void)=NULL;
/*****************************************
---------------    API     ---------------
*****************************************/
/****************************************************************************************************
* Function Name   : Get_Task_TCB
* Description     : Retrieves the Task Control Block (TCB) for a specified task entry function.
* Sync-Async      : Synchronous
* Reentrancy      : Reentrant (Assuming that `RTOS_Control.Tasks` and `RTOS_Control.Tasks_Count` are not modified concurrently by different threads or interrupts.)
* Parameters (in) : Task_Entry - Pointer to the task entry function for which the TCB is to be retrieved.
* Parameters (out): None
* Return value    : Pointer to the TCB corresponding to the task entry function. 
*                   If not found, it returns the TCB after the last valid entry (potentially out of bounds).
* Notes           : - The function iterates through the global task array `RTOS_Control.Tasks` and checks each task's entry function.
*                   - If a match is found with the provided `Task_Entry`, it breaks out of the loop and returns the corresponding TCB.
*                   - Care should be taken when using this function. If no match is found, the function returns a potentially out-of-bounds TCB.
*                   - It's recommended to always verify the task's existence before using the returned TCB in other operations.
*****************************************************************************************************/
static RTOS_TCB_t* Get_Task_TCB (void (*Task_Entry)(void))
{
    u8 Counter = ZERO;
    /* Loop through all tasks in the system */
    for(Counter = ZERO; Counter < RTOS_Control.Tasks_Count; Counter++)
    {
        /* If the entry point matches, break out of the loop */
        if(Task_Entry == RTOS_Control.Tasks[Counter].Task.Task_Entry) { break; }
    }
    return &RTOS_Control.Tasks[Counter];
}
/********************************************************************
* Syntax          : static void RTOS_Create_Task_Stack(RTOS_TCB_t* Task_TCB)
* Description     : Initializes the stack for the specified task, setting up both hardware and software stack frames.
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Task_TCB - Pointer to the Task Control Block (TCB) of the task for which the stack is being initialized.
* Parameters (out): None
* Return value    : Void
* Notes           : This function prepares the task's stack to ensure correct context switching. The stack frame includes 
*                   hardware and software saved registers. The stack is initialized in such a way that the first context switch 
*                   will lead to the task's entry function being executed.
********************************************************************/
static void RTOS_Create_Task_Stack(RTOS_TCB_t* Task_TCB)
{
/*
====================================
------------ By Hardware -----------
====================================
     * XPSR    : (Core_Register_Interrupt_number)
     * PC      : (Program_Counter)
     * LR      : (Linkk Register "MSP-PSP Return Info")
     * R12     : ()
     * R4      : ()
     * R3      : ()
     * R2      : (Argument_3)
     * R1      : (Argument_2)
     * R0      : (Argument_1,Return Value)
====================================
------------ By Software -----------
====================================
     * R5      : ()
     * R6      : ()
     * R7      : ()
     * R8      : ()
     * R9      : ()
     * R10     : ()
     * R11     : ()
*/
     u8 Counter;
     /*Initialize SP In this Task Stack*/
     Task_TCB->Current_Task_PSP=(u32*)Task_TCB->Start_PSP_Stack;
     /*Decrement SP & Store XPSR*/
     Task_TCB->Current_Task_PSP--;
     (*(Task_TCB->Current_Task_PSP))=0x01000000;
     /*Decrement SP & Store Task PC*/
     Task_TCB->Current_Task_PSP--;
     (*(Task_TCB->Current_Task_PSP))=(u32)Task_TCB->Task.Task_Entry;
     /*Decrement SP & Store LR*/
     Task_TCB->Current_Task_PSP--;
     (*(Task_TCB->Current_Task_PSP))=0xFFFFFFFD;
     /*Decrement SP & Store General Purpose Registers*/
     for(Counter=ZERO;Counter<13;Counter++){Task_TCB->Current_Task_PSP--;(*(Task_TCB->Current_Task_PSP))=ZERO;}
}
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
static void RTOS_Initialize_Stack(void)
{
     /*Initialize MSP Start*/
     RTOS_Control.Start_MSP_Stack=(u32)&Top_Of_Stack;
     /*Initialize MSP End*/
     RTOS_Control.End_MSP_Stack=RTOS_Control.Start_MSP_Stack-Main_Stack_Size;
     /*Initialize PSP For Tasks*/
	RTOS_Control.Current_PSP=(u32*)(RTOS_Control.End_MSP_Stack-8);
}
/********************************************************************
* Syntax          : static void __attribute__((noreturn)) RTOS_Kernal_Idle_Task(void)
* Description     : Kernel's internal idle task that continuously checks and executes the user-defined idle task.
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void (Function never returns due to the `noreturn` attribute)
* Notes           : - This function is the default behavior for the idle state of the RTOS kernel.
*                   - It continuously checks if there's a user-defined idle task callback and executes it if present.
********************************************************************/
static void __attribute__((noreturn)) RTOS_Kernal_Idle_Task(void)
{
     while (1)
     {
          /*Check If Idle Task Is Initialized Start Execution*/
          if(NULL!=Idle_Task_CallBack){Idle_Task_CallBack();}
          else{}
          /*Wait For Interrupt Or Event*/
          Wait_For_Event;
     }
}
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
void RTOS_Idle_Task(void (*Idle_Task)(void))
{
     if(NULL!=Idle_Task){Idle_Task_CallBack=Idle_Task;}
}
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
void __attribute__((noreturn)) RTOS_Run_Tasks(void)
{
     RTOS_TCB_t* Idle_Task_TCB=NULL;
     Idle_Task_TCB=Get_Task_TCB(RTOS_Kernal_Idle_Task);
     /*Update Running Task To Be Idle Task*/
     RTOS_Control.Running_Task=Idle_Task_TCB;
     /*Activate Idle Task*/
     RTOS_Kernal_Activate_Task(Idle_Task_TCB);
     /*Update RTOS State To Running*/
     RTOS_Control.Mode=RTOS_Running;
     /*Enable Scheduler Timer*/
     SysTick_Configuration(8000);
     /*Set PSP To Start Of Idle Task Stack*/
     SET_PSP(RTOS_Control.Running_Task->Current_Task_PSP);
     /*Switch Stack Pointer To Shadow Register PSP*/
     Switch_PSP;
     /*Swithch To Unprivileged To Start Execution In Idle Task*/
     Switch_Unprivileged;
     /*Go to Idle Task Entry*/
     RTOS_Kernal_Idle_Task();
}
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
void RTOS_Create_Task(RTOS_Task_t Task)
{
     /*If Available Stack Thnen Store Task DATA*/
     RTOS_Control.Tasks[RTOS_Control.Tasks_Count].Task=Task;
     /*Define Start Point Of Task Stack*/
     RTOS_Control.Tasks[RTOS_Control.Tasks_Count].Start_PSP_Stack=(u32)RTOS_Control.Current_PSP;
     /*Update PSP Locator For The New Task & Aligne 16 Byte Between Tasks*/
     RTOS_Control.Current_PSP-=(RTOS_Control.Tasks[RTOS_Control.Tasks_Count].Task.Stack_Size+8);
     /*Initialize Stack For This Task*/
     RTOS_Create_Task_Stack(&RTOS_Control.Tasks[RTOS_Control.Tasks_Count]);
     /*Update State*/
     RTOS_Control.Tasks[RTOS_Control.Tasks_Count].State=RTOS_State_Suspend;
     /*Configure Task As Not Waiting State*/
     RTOS_Control.Tasks[RTOS_Control.Tasks_Count].Waiting_Time.State=Not_Waiting;
     /*Increment Task Counter*/
     RTOS_Control.Tasks_Count++;
}
/********************************************************************
* Syntax          : RTOS_Initialize(void)
* Description     : Initialize the RTOS environment, setting up the idle task and initializing necessary resources.
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
* Notes           : - Creates the Idle Task with specified attributes
*                   - Sets the task counter to zero (No tasks are created yet)
*                   - Enables the RTOS in suspend state
*                   - Initializes the hardware clock related to RTOS
*                   - Initializes the main stack size for RTOS
*                   - Initializes the ready queue for tasks
*                   - Creates the IDLE task for RTOS
********************************************************************/
void RTOS_Initialize(void)
{
     /*Create Idle Task*/
     RTOS_Task_t Idle_Task={255,"Idle_Task",Idle_Task_Stack_Size,RTOS_Kernal_Idle_Task};
     /*Set Task Counter To ZERO No Tasks Are Created*/
     RTOS_Control.Tasks_Count=ZERO;
     /*Enable Suspend State*/
     RTOS_Control.Mode=RTOS_Suspend;
     /*Initialize Related Hardware Clock*/
     Hardware_Initialization();
     /*Initialize Main Stack Size*/
     RTOS_Initialize_Stack();
     /*Initialize Ready Queue*/
     Queue_Initialization(&Ready_Queue);
     /*Create Idle Task IDLE Task*/
     RTOS_Create_Task(Idle_Task);
}
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
void RTOS_Mutex_Aquire(RTOS_Mutex_t* Mutex)
{
    if(Mutex->Owner_Entry_Function == NULL)
    {
        Mutex->Owner_Entry_Function = RTOS_Control.Running_Task->Task.Task_Entry;
    }
    else
    {
        if(Mutex->Aquire_Entry_Function == NULL)
        {
            Mutex->Aquire_Entry_Function = RTOS_Control.Running_Task->Task.Task_Entry;
            RTOS_Control.Running_Task->State = RTOS_State_Suspend;
            RTOS_Service(RTOS_Service_Terminate_Task);
        }
        else {}
    }
}
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
void RTOS_Mutex_Release(RTOS_Mutex_t* Mutex)
{
    if(Mutex->Aquire_Entry_Function != NULL)
    {
        if(Mutex->Owner_Entry_Function == RTOS_Control.Running_Task->Task.Task_Entry)
        {
            Mutex->Owner_Entry_Function = Mutex->Aquire_Entry_Function;
            Mutex->Aquire_Entry_Function = NULL;
            Get_Task_TCB(Mutex->Owner_Entry_Function)->State = RTOS_State_Waiting;
            RTOS_Control.Running_Task->State = RTOS_State_Waiting;
            RTOS_Service(RTOS_Service_Activate_Task);
        }
        else {}
    }
    else {}
}
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
void RTOS_Activate_Task(RTOS_Task_t Task)
{
     RTOS_TCB_t * Test=NULL;
     Test=Get_Task_TCB(Task.Task_Entry);
     RTOS_Control.Running_Task->State=RTOS_State_Waiting;     
     RTOS_Kernal_Activate_Task(Test);
}
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
void RTOS_Terminate_Task(RTOS_Task_t Task)
{
     RTOS_Control.Running_Task->State=RTOS_State_Waiting;
     RTOS_Kernal_Terminate_Task(Get_Task_TCB(Task.Task_Entry));
}
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
void RTOS_Delay(u32 Ticks_Count)
{
     RTOS_Control.Running_Task->Waiting_Time.State=Waiting;
     RTOS_Control.Running_Task->Waiting_Time.Ticks_Number=Ticks_Count-ONE;
     RTOS_Control.Running_Task->State=RTOS_State_Suspend;
     RTOS_Service(RTOS_Service_Terminate_Task);
}
/********************************************************************
 *  END OF FILE:  RTOS.c
********************************************************************/
