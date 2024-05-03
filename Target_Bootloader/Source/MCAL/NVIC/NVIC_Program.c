/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  NVIC_Program.c
 *  Module:  MCAL/NVIC
 *  Description:  Nested Victor Interrupt Controller Driver Program File For STM32F103C6
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "NVIC_Interface.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/********************************************************************
* Syntax          : MCAL_GPIO_Lock_Pin(GPIOX_t *Port,GPIO_Pin_t Pin)
* Description     : Lock State For Specific Pin
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(Copy To Pin Number "Pin_Number_t")
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void NVIC_Interrupt(NVIC_Interrupt_Numbers_t Interrupt_Number,logic_t State)
{
     /*Enable Disable Interrupt*/
     if(State==Enable)Set_Bit(NVIC.ISERX[Interrupt_Number/32].Register,(Interrupt_Number%32));
     else Set_Bit(NVIC.ICERX[Interrupt_Number/32].Register,(Interrupt_Number%32));
}
/********************************************************************
 *  END OF FILE:  NVIC_Program.c
********************************************************************/
