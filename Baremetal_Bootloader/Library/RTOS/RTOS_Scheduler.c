/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  RTOS_Scheduler.c
 *  Module:  Library/RTOS
 *  Description:  Scheduler Algorithm To Run My Real Time Operating System
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "RTOS_Scheduler.h"
#include "RTOS_Porting.h"
#include "Queue.h"
/*****************************************
-----------    Extern Values   -----------
*****************************************/
extern RTOS_Control_t RTOS_Control;
extern Queue_t Ready_Queue;
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/*Enable Context Swith Hook Function For Testing*/
#ifdef Enable_Context_Switch_Hook_Function
static void (*Hook_Function_Call_Back)(void)=NULL;
#endif
/****************************************************************************************************
* Function Name   : RTOS_Update_Waiting_Time
* Description     : Iterates through each task in the scheduler table and updates the waiting time for tasks that are in a waiting state.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Assumed based on manipulation of global data. Ensure restricted access in a multithreaded environment.)
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
* Notes           : - The function loops through all tasks in the scheduler table to check their waiting state.
*                   - If a task's waiting time has reached its end (indicated by a tick number of ONE), 
*                     it changes the task's state and invokes the RTOS service to handle the task that was waiting.
*                   - If the task's waiting time has not yet concluded, it decrements the task's waiting time tick counter.
*                   - The function operates on the global `RTOS_Control` structure which contains the list of tasks and their states.
*****************************************************************************************************/
static void RTOS_Update_Waiting_Time(void)
{
	/*Loop On Each Task In Scheduler Table*/
	for(u8 Counter = 0; Counter < RTOS_Control.Tasks_Count; Counter++)
	{
		/*Check If This Task Is Waiting Or Not*/
		if(Waiting == RTOS_Control.Tasks[Counter].Waiting_Time.State)
		{
			/*Check If Waiting Time Is Finished*/
			if(RTOS_Control.Tasks[Counter].Waiting_Time.Ticks_Number == ONE)
			{
				/*Activate Its State And Run Scheduler*/
				RTOS_Control.Tasks[Counter].State = RTOS_State_Waiting;
				RTOS_Control.Tasks[Counter].Waiting_Time.State = Not_Waiting;
				RTOS_Control.Running_Task->State = RTOS_State_Waiting;
				RTOS_Service(RTOS_Service_Waiting_For_Task);
			}
			/*Task Time Still Not Finished*/
			else
			{
				/*Decrement Time Counter Of This Specific Task*/
				RTOS_Control.Tasks[Counter].Waiting_Time.Ticks_Number--;
			}
		}
	}
}
/****************************************************************************************************
* Function Name   : RTOS_Bubble_Sort
* Description     : Sorts the tasks based on their priorities using the bubble sort algorithm. 
*                   Tasks with lower priority values will be placed first in the array.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Sorting operations are inherently not thread-safe. Ensure access restrictions in a multithreaded environment.)
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
* Notes           : - The function ensures that tasks are executed in order of their priority.
*                   - If during an iteration no swaps are made, it means the list is already sorted 
*                     and the function breaks out of the loop.
*                   - Temporary_TCB is used to swap tasks when out of order.
*                   - The function operates on the global `RTOS_Control` structure which contains the list of tasks.
*****************************************************************************************************/
static void RTOS_Bubble_Sort(void)
{
     s8 Swap_Flag,Counter,Iteration;
	RTOS_TCB_t Temporary_TCB;
	for(Counter=((s8)(RTOS_Control.Tasks_Count-ONE));Counter>=ZERO;Counter--)
	{
		Swap_Flag=TRUE;
		for(Iteration=ZERO;Iteration<Counter;Iteration++)
		{
			if(RTOS_Control.Tasks[Iteration].Task.Priority>RTOS_Control.Tasks[Iteration+ONE].Task.Priority)
			{
				Temporary_TCB=RTOS_Control.Tasks[Iteration];
				RTOS_Control.Tasks[Iteration]=RTOS_Control.Tasks[Iteration+ONE];
				RTOS_Control.Tasks[Iteration+ONE]=Temporary_TCB;
				Swap_Flag=FALSE;
			}
		}
		if(Swap_Flag)break;
	}
}
/****************************************************************************************************
* Function Name   : RTOS_Select_Next_Task
* Description     : Selects the next task to run based on the current state of the system and the tasks in the ready queue.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Assumed based on manipulation of global data. Ensure restricted access in a multithreaded environment.)
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
* Notes           : - The function examines the current state of the running task and the size of the ready queue.
*                   - If the current task is not suspended and the ready queue is empty, the current task continues to run.
*                   - If the ready queue is not empty, the task at the head of the queue is set to run.
*                   - If the priority of the currently running task is the same as the priority of the next task in the queue, 
*                     and the current task is not suspended, the current task is moved to the ready state.
*                   - The function works with the global `RTOS_Control` structure which holds information about tasks and their states.
*****************************************************************************************************/
static void RTOS_Select_Next_Task(void)
{
	/*Check Ready Queue Is Embty && Current Task Not Suspend*/
	if((RTOS_State_Suspend!=RTOS_Control.Running_Task->State)&&(ZERO==Ready_Queue.size))
	{
		/*Ensure Running Task State Is Running*/
		RTOS_Control.Running_Task->State=RTOS_State_Running;
		/*Push Currint Task To Ready Queu*/
		Queue_Enqueue(&Ready_Queue,RTOS_Control.Running_Task);
		/*Set Bending Task To Running*/
		RTOS_Control.Pending_Task=RTOS_Control.Running_Task;
	}
	else
	{
		/*Dequeue The Next Task Sould Run In Pending Task Pointer*/
		Queue_Dequeue(&Ready_Queue,&RTOS_Control.Pending_Task);
		/*Change Its State To Running*/
		RTOS_Control.Pending_Task->State=RTOS_State_Running;
		/*Check If Running And Pending Is Same Priority And Running Not Suspend*/
		if((RTOS_Control.Running_Task->Task.Priority==RTOS_Control.Pending_Task->Task.Priority)&&(RTOS_State_Suspend!=RTOS_Control.Running_Task->State))
		{
			if(RTOS_Control.Running_Task->Task.Task_Entry!=RTOS_Control.Pending_Task->Task.Task_Entry)
			{
				/*Move Running Task To Ready Queue And Change Its State To Ready*/
				Queue_Enqueue(&Ready_Queue,RTOS_Control.Running_Task);
				RTOS_Control.Running_Task->State=RTOS_State_Ready;
			}
		}
	}
}
/****************************************************************************************************
* Function Name   : SVC_Number
* Description     : Handles the supervisor calls (SVC) based on the provided service number.
* Sync-Async      : Synchronous
* Reentrancy      : Reentrant (Assumed based on code structure. Validate depending on the specific supervisor calls.)
* Parameters (in) : Stack_Pointer - Pointer to the current stack, which contains the state of the system and the SVC number.
* Parameters (out): None
* Return value    : Void
* Notes           : 
*                   - The function checks the SVC number stored in the instruction preceding the one at the Program Counter (PC).
*                   - Depending on the extracted SVC number, different actions are taken.
*****************************************************************************************************/
void SVC_Number(u32* Stack_Pointer)
{
	switch((((u8*)(Stack_Pointer[6]))[-2]))
	{
		case RTOS_Service_Activate_Task:
		case RTOS_Service_Terminate_Task:
			/*Update Scheduler Table And Ready Queue With New Tasks After Changing Its States*/
            	RTOS_Update_Scheduler_Table();
			/*Check If RTOS Is Running And Running Task Not Idle Task*/
			if(RTOS_Running==RTOS_Control.Mode)
			{
#ifdef Enable_Context_Switch_Hook_Function
				Hook_Function_Call_Back();
#endif
				/*Select Next Task Should Be Run*/
				RTOS_Select_Next_Task();
				/*Trigeers Pend-SV To Do Context Swithc If Thre Was Any Chnange*/
				PendSV_Request;
			}
		break;
		case RTOS_Service_Waiting_For_Task:
			/*Update Scheduler Table To Update Task After FinishIts Waiting Time*/
			RTOS_Update_Scheduler_Table();
		break;
	}
}
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
void RTOS_Update_Scheduler_Table(void)
{
	/*Update Ready Queue*/
	RTOS_TCB_t* Task_TCB_Pointer=NULL;
	u8 Counter=ZERO,Priority=255;
	/*Bubble Sort Scheduler Table Based On Priority*/
	RTOS_Bubble_Sort();
	/*Free Ready Queue*/
	while(Queue_Ok==Queue_Dequeue(&Ready_Queue,&Task_TCB_Pointer)){}
	/*Update Ready Queue*/
	for(Counter=ZERO;Counter<RTOS_Control.Tasks_Count;Counter++)
	{
		/*Check For The State Of The Task Is Not Suspend*/
		if((RTOS_Control.Tasks[Counter].State!=RTOS_State_Suspend)&&(RTOS_Control.Tasks[Counter].State!=RTOS_State_Running))
		{
			/*Pick Up The Heist Priority To Run*/
			if((RTOS_Control.Tasks[Counter].Task.Priority<=Priority))
			{
				/*Update Priority According To Hiest One*/
				Priority=RTOS_Control.Tasks[Counter].Task.Priority;
				/*Enqueue Every Task With This Priority*/
				Queue_Enqueue(&Ready_Queue,&RTOS_Control.Tasks[Counter]);
				/*Change This Task State To Be Ready*/
				RTOS_Control.Tasks[Counter].State=RTOS_State_Ready;
			}
			/*If he Next Task Priority Is Low Then Break*/
			else{break;}
		}
		else{}
	}
}
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
void RTOS_Service(RTOS_Service_t Service_ID)
{
	switch (Service_ID)
	{
		case RTOS_Service_Activate_Task:Supervisor_Call(Activate_Task);break;
		case RTOS_Service_Terminate_Task:Supervisor_Call(Terminate_Task);break;
		case RTOS_Service_Waiting_For_Task:Supervisor_Call(Waiting_Task);break;
	}
}
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
void RTOS_Kernal_Activate_Task(RTOS_TCB_t* Task_TCB)
{
	/*Update State To Waiting*/
	Task_TCB->State=RTOS_State_Waiting;
	/*Update Scheduler Table*/
	RTOS_Service(RTOS_Service_Activate_Task);
}
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
void RTOS_Kernal_Terminate_Task(RTOS_TCB_t* Task_TCB)
{
	/*Update State To Suspend*/
	Task_TCB->State=RTOS_State_Suspend;
	/*Update Scheduler Table*/
	RTOS_Service(RTOS_Service_Terminate_Task);
}
/****************************************************************************************************
* Function Name   : RTOS_Context_Switch_Hook_Function
* Description     : Registers a hook function that gets called during a context switch, if the context switch hook functionality is enabled.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Assumed based on potential access to global data. Ensure restricted access in a multithreaded environment.)
* Parameters (in) : Hook_Function - Pointer to the hook function to be called during a context switch.
* Parameters (out): None
* Return value    : Void
* Notes           : - The hook function is intended to provide a mechanism to execute custom code during a context switch.
*                   - If the provided Hook_Function is not NULL, it is registered as the hook function to be called during context switches.
*                   - The function operates conditionally, only if the `Enable_Context_Switch_Hook_Function` is defined.
*                   - The global `Hook_Function_Call_Back` is assigned the passed `Hook_Function`.
*****************************************************************************************************/
#ifdef Enable_Context_Switch_Hook_Function
void RTOS_Context_Switch_Hook_Function(void (*Hook_Function)(void))
{
	if(NULL != Hook_Function) { Hook_Function_Call_Back = Hook_Function; }
}
#endif
/****************************************************************************************************
* Function Name   : SysTick_Handler
* Description     : Interrupt service routine for the System Tick timer. Responsible for selecting the next task and triggering a context switch.
* Sync-Async      : Asynchronous (Interrupt Service Routine)
* Reentrancy      : Non-Reentrant (As it's an ISR and should be completed before another can occur in most cases.)
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
* Notes           : - The function is called every system tick, as defined by the SysTick timer configuration.
*                   - It is responsible for updating the currently running task by selecting the next task.
*                   - After selecting the next task, a context switch is requested by triggering the PendSV interrupt.
*****************************************************************************************************/
void SysTick_Handler(void)
{
	/*Update Waiting Time For Tasks*/
	RTOS_Update_Waiting_Time();
	/*Select Next Task*/
	RTOS_Select_Next_Task();
	/*Trigger PendSV If There Is Context Swith*/
	if(RTOS_Control.Running_Task!=RTOS_Control.Pending_Task)
	{
/*If Context Switch Hook Function Is Enabled Fall Finction*/
#ifdef Enable_Context_Switch_Hook_Function
		if(NULL!=Hook_Function_Call_Back){Hook_Function_Call_Back();}
#endif
		PendSV_Request;
	}
	else {}
}
/****************************************************************************************************
* Function Name   : PendSV_Handler
* Description     : Interrupt service routine for PendSV interrupt. Responsible for saving and restoring tasks' context during context switching.
* Sync-Async      : Asynchronous (Interrupt Service Routine)
* Reentrancy      : Non-Reentrant (As it's an ISR. Ensure restricted access to avoid potential reentrancy issues.)
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void (the function returns through an assembly "BX LR" instruction)
* Notes           : - This ISR is marked as "naked", which means the compiler will not generate any prologue or epilogue code.
*                   - The function first saves the context (R4-R11 registers) of the currently running task.
*                   - It then switches the control to the pending task (if available) or continues with the current task.
*                   - The context of the task that is set to run is then restored (R4-R11 registers).
*                   - The stack pointer (PSP) is updated to reflect the new task's context.
*                   - The function concludes by returning from the interrupt through an assembly instruction.
*                   - The function makes extensive use of assembly instructions to interact directly with CPU registers.
*****************************************************************************************************/
void __attribute((naked)) PendSV_Handler(void)
{
    	/*Get Current PSP From CPU Registers*/
	GET_PSP(RTOS_Control.Running_Task->Current_Task_PSP);
/*----------------- Store R4 To R11 --------------------*/
	Decrement_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_From_Register((*(RTOS_Control.Running_Task->Current_Task_PSP)),r4);
	Decrement_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_From_Register((*(RTOS_Control.Running_Task->Current_Task_PSP)),r5);
	Decrement_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_From_Register((*(RTOS_Control.Running_Task->Current_Task_PSP)),r6);
	Decrement_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_From_Register((*(RTOS_Control.Running_Task->Current_Task_PSP)),r7);
	Decrement_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_From_Register((*(RTOS_Control.Running_Task->Current_Task_PSP)),r8);
	Decrement_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_From_Register((*(RTOS_Control.Running_Task->Current_Task_PSP)),r9);
	Decrement_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_From_Register((*(RTOS_Control.Running_Task->Current_Task_PSP)),r10);
	Decrement_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_From_Register((*(RTOS_Control.Running_Task->Current_Task_PSP)),r11);
/*------------ Store Context Of Next Task --------------*/
/*Switch To Pending Task*/
	__asm volatile                                     
	(                                                  
		"LDR r0, [%[PendingTaskPtr]]   \n\t"           
		"CMP r0, #0                    \n\t"           
		"BEQ Not_Equal                 \n\t"           
		"STR r0, [%[RunningTaskPtr]]   \n\t"           
		"MOV r1, #0                    \n\t"           
		"STR r1, [%[PendingTaskPtr]]   \n\t"           
		"Not_Equal:                    \n\t"           
		:                                              
		: [PendingTaskPtr] "r" (&RTOS_Control.Pending_Task),
		  [RunningTaskPtr] "r" (&RTOS_Control.Running_Task) 
		: "r0", "r1"                                   
	);
/*---------------- ReStore R4 To R11 -------------------*/
	Move_To_Register(r11,(*(RTOS_Control.Running_Task->Current_Task_PSP)));
	Increment_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_To_Register(r10,(*(RTOS_Control.Running_Task->Current_Task_PSP)));
	Increment_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_To_Register(r9,(*(RTOS_Control.Running_Task->Current_Task_PSP)));
	Increment_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_To_Register(r8,(*(RTOS_Control.Running_Task->Current_Task_PSP)));
	Increment_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_To_Register(r7,(*(RTOS_Control.Running_Task->Current_Task_PSP)));
	Increment_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_To_Register(r6,(*(RTOS_Control.Running_Task->Current_Task_PSP)));
	Increment_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_To_Register(r5,(*(RTOS_Control.Running_Task->Current_Task_PSP)));
	Increment_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	Move_To_Register(r4,(*(RTOS_Control.Running_Task->Current_Task_PSP)));
	Increment_Value(RTOS_Control.Running_Task->Current_Task_PSP);
	/*Update PSP To Start Execute*/
	SET_PSP(RTOS_Control.Running_Task->Current_Task_PSP);
	/*Manual Return*/
	__asm volatile("BX LR");
}
/********************************************************************
 *  END OF FILE:  RTOS_Scheduler.c
********************************************************************/
