/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  RTOS_Porting.h
 *  Module:  Library/RTOS
 *  Description:  Porting Hidder File To Run My Real Time Operating System
*******************************************************************/
#ifndef _RTOS_PORTING_H_
#define _RTOS_PORTING_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "STM32F103.h"
/*****************************************
---------    Default Handler    ---------
*****************************************/
__attribute__((noreturn)) void NMI_Handler(void);
__attribute__((noreturn)) void HardFault_Handler(void);
__attribute__((noreturn)) void MemManage_Handler(void);
__attribute__((noreturn)) void BusFault_Handler(void);
__attribute__((noreturn)) void UsageFault_Handler(void);
/*****************************************
---------    Type Definitions    ---------
*****************************************/
#define Supervisor_Call(Number)                   SVC_HELPER(Number)
#define PendSV_Request                            SCB.ICSR.Bits.Bit_28=Enable
#define Increment_Value(Value)                    __asm volatile ("LDR r0, %[PtrAddr]\n\tADD r0, r0, #4\n\tSTR r0, %[PtrAddr]\n\t"::[PtrAddr]"m"(Value):"r0")
#define Decrement_Value(Value)                    __asm volatile ("LDR r0, %[PtrAddr]\n\tSUB r0, r0, #4\n\tSTR r0, %[PtrAddr]\n\t"::[PtrAddr]"m"(Value):"r0")
#define Move_From_Register(Value, Register)       __asm volatile ("STR "#Register",%[Output]":[Output]"=m"(Value))
#define Move_To_Register(Register,Value)          __asm volatile ("LDR "#Register",%[Argument]"::[Argument]"m"(Value))
#define SET_PSP(Task_Address)			          __asm volatile ("MSR PSP,%[Variable]"::[Variable]"r"(Task_Address))
#define GET_PSP(Task_Address)			          __asm volatile ("MRS %[Variable],PSP":[Variable]"=r"(Task_Address))
#define GET_XPSR(Variable)		               __asm volatile ("MRS %[Argument],XPSR":[Argument]"=r"(Variable))
#define Switch_MSP					          __asm volatile ("MRS R0,CONTROL\n\tBFC R0,#1,#1\n\tMSR CONTROL,R0":::"r0")
#define Switch_PSP					          __asm volatile ("MRS R0,CONTROL\n\tORR R0,R0,#2\n\tMSR CONTROL,R0":::"r0")
#define Switch_Unprivileged			          __asm volatile ("MRS R0,CONTROL\n\tORR R0,R0,#1\n\tMSR CONTROL,R0":::"r0")
#define Switch_Privileged			          __asm volatile ("MRS R0,CONTROL\n\tBFC R0,#0,#1\n\tMSR CONTROL,R0":::"r0")
#define SVC_HELPER(Number)                        __asm volatile ("SVC #"#Number)
#define Busy_Wait		                         __asm volatile ("NOP")
#define Wait_For_Event		                    __asm volatile ("WFE")
/*****************************************
---------    Configure Macros    ---------
*****************************************/
#define SysTick_IRQn                    (-1)
#define PendSV_IRQn                     (-2)
/*****************************************
---------    Global Variables    ---------
*****************************************/
extern u32 Top_Of_Stack;
extern u32 __end_sp;
/*****************************************
---  Application Programming Interface  --
*****************************************/
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
void SVC_Number(u32* Stack_Pointer);
/****************************************************************************************************
* Function Name   : Hardware_Initialization
* Description     : Initializes the system hardware settings including PendSV priority and system clock.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Assumed based on hardware register manipulation. Ensure restricted access in a multithreaded environment.)
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
* Notes           : - The function performs a series of initialization tasks to set up the system hardware for operation.
*                   - PendSV, which stands for "Pendable request for system service," is given the lowest priority (priority value of 15).
*                   - The function also configures the system clock. Initially, the high-speed internal oscillator (HSI) is enabled, while other oscillators and phase-locked loops (PLL) are turned off.
*                   - The clock configuration register (CFGR) is reset to its default state.
*                   - Finally, peripheral reset registers for both APB1 and APB2 buses are reset, ensuring that no peripherals are held in a reset state.
*****************************************************************************************************/
void Hardware_Initialization(void);
/****************************************************************************************************
* Function Name   : SVC_Handler
* Description     : Handles the Supervisor Call (SVC) interrupt, which is a service call interrupt.
*                   It determines which stack the thread was using (MSP or PSP) and branches to the 
*                   appropriate SVC number handling routine.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (It's an interrupt service routine, so reentrancy depends on the interrupt settings.)
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
* Notes           : - The function uses the TST instruction to check if bit #4 of the LR (Link Register) is set.
*                   - If bit #4 of LR is set, it indicates the thread was using the process stack (PSP). 
*                     Otherwise, it was using the main stack (MSP).
*                   - Depending on the result, either MSP or PSP is loaded into r0.
*                   - Following this, the function branches to SVC_Number, which presumably handles 
*                     various SVC numbers and their respective tasks.
*****************************************************************************************************/
void __attribute((naked)) SVC_Handler(void);
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
void PendSV_Handler(void);
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
void SysTick_Handler(void);
/****************************************************************************************************
* Function Name   : SysTick_Configuration
* Description     : Configures the SysTick timer for a specified tick duration and initializes its settings.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant (Assumed based on hardware register manipulation. Ensure restricted access in a multithreaded environment.)
* Parameters (in) : ticks - The number of ticks for SysTick timer to count before triggering an interrupt.
* Parameters (out): None
* Return value    : Void
* Notes           : - This function configures the SysTick timer, which is a dedicated system timer present in ARM Cortex-M processors.
*                   - The reload value for the SysTick timer is set to the specified tick duration minus 1 (because the counter counts from 0).
*                   - The current value of the SysTick timer is reset to 0.
*                   - SysTick interrupt priority is set to the least urgency (highest numeric priority value).
*                   - The function then configures the SysTick control register to use the processor clock, and enables both the SysTick timer and its interrupt.
*****************************************************************************************************/
void SysTick_Configuration(u32 ticks);
/********************************************************************
 *  END OF FILE:  RTOS_Porting.h
********************************************************************/
#endif
