/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  RTOS_Porting.c
 *  Module:  Library/RTOS
 *  Description:  Porting C File To Run My Real Time Operating System
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "RTOS_Porting.h"
/*****************************************
---------    Default Handler    ---------
*****************************************/
__attribute__((noreturn)) void NMI_Handler(void)
{
    while(TRUE){}
}
__attribute__((noreturn)) void HardFault_Handler(void)
{
    while(TRUE){}
}
__attribute__((noreturn)) void MemManage_Handler(void)
{
    while(TRUE){}
}
__attribute__((noreturn)) void BusFault_Handler(void)
{
    while(TRUE){}
}
__attribute__((noreturn)) void UsageFault_Handler(void)
{
    while(TRUE){}
}
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
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
void __attribute((naked)) SVC_Handler(void)
{
	__asm
	(
		"TST LR,#4\n\t"
		"ITE EQ\n\t"
		"MRSEQ r0,MSP\n\t"
		"MRSNE r0,PSP\n\t"
		"B SVC_Number"
	);
}
/****************************************************************************************************
* Function Name   : NVIC_SetPriority
* Description     : Sets the priority of a specific interrupt.
*                   Currently supports setting priority for SysTick and PendSV interrupts.
* Sync-Async      : Synchronous
* Reentrancy      : Reentrant
* Parameters (in) : IRQn     - The IRQ number of the interrupt. Expected values are SysTick_IRQn and PendSV_IRQn.
*                   priority - The priority value to set. Higher numeric values mean lower priority.
* Parameters (out): None
* Return value    : Void
* Notes           : - The priority setting for each interrupt is located at different bit positions 
*                     within the System Control Block's (SCB) SHPR (System Handler Priority Register).
*                   - For SysTick, the priority setting is located in SHPR3 with a 24-bit offset.
*                   - For PendSV, the priority setting is in SHPR3 with a 16-bit offset.
*                   - The function masks and sets the appropriate bits to configure the priority.
*****************************************************************************************************/
static void NVIC_SetPriority(s32 IRQn, u32 priority)
{
    if (IRQn == SysTick_IRQn)
    {
        /* SysTick priority setting in SHPR3 with 24-bit offset */
        SCB.SHPR[2].Register = (SCB.SHPR[2].Register & ~(0xFFUL << 24U)) | (priority << 28U);
    }
    else if (IRQn == PendSV_IRQn)
    {
        /* PendSV priority setting in SHPR3 with 16-bit offset */
        SCB.SHPR[2].Register = (SCB.SHPR[2].Register & ~(0xFFU << 16U)) | (priority << 20U);
    }
    else {}
}
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
void SysTick_Configuration(u32 ticks)
{
    /* Set the SysTick reload value */
    SysTick.LOAD.Register = ticks - 1U;
    /* Reset SysTick current value */
    SysTick.VAL.Register=0U;
    /* Set SysTick interrupt priority to the least urgency (highest priority value) */
    NVIC_SetPriority(SysTick_IRQn,(1U<<4U)-1U);
    /* Select the processor clock, and enable SysTick and its interrupts */
    SysTick.CTRL.Register=0x07;
}
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
void Hardware_Initialization(void)
{
/*--------- Configure PendSV Priority ------------*/
    /* Set the PendSV priority to 15 */
    NVIC_SetPriority(PendSV_IRQn,15);
/*----------- Configure System Clock -------------*/
    /*1.Set HSION bit and clear HSEON, CSSON and PLLON bits*/
    Set_Bit(RCC.CR.Register,ZERO);
    RCC.CR.Register &= (u32)~(0x00010000|0x00080000|0x01000000);
    /*2.Reset CFGR register (set to default value after reset)*/
    RCC.CFGR.Register &= 0xF8FF0000;
    /*3.Reset the RCC_APB1RSTR and RCC_APB2RSTR registers to disable the reset of all peripherals*/
    RCC.APB1RSTR.Register=0;
    RCC.APB2RSTR.Register=0;
}
/********************************************************************
 *  END OF FILE:  RTOS_Porting.c
********************************************************************/
