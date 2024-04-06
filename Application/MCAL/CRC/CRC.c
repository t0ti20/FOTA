/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  CRC.c
 *  Module:  MCAL/CRC
 *  Description:  CRC calculation unit Program File For STM32F103C6
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "RCC_Interface.h"
#include "CRC.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/********************************************************************
* Syntax          : u32 CRC_Accumulate(u32 Value)
* Description     : Feed a 32-bit value to the CRC computation unit and
*                   immediately return the updated CRC value.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant
* Parameters (in) : 32-bit data word to be included in the CRC computation.
* Parameters (out): None
* Return value    : u32 - The updated CRC value after the computation.
********************************************************************/
u32 CRC_Accumulate(u32 Value)
{
     CRC.DR.Register = Value;
     return CRC.DR.Register;
}
/********************************************************************
* Syntax          : CRC_Reset(void)
* Description     : Resets the CRC computation to its initial state.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
********************************************************************/
void CRC_Reset(void)
{
     CRC.CR.Bits.Bit_0=Enable;
}
/********************************************************************
* Syntax          : CRC_Initialization(void)
* Description     : Initialize the CRC module for computations.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
********************************************************************/
void CRC_Initialization(void)
{
     RCC_Clock(RCC_CRC,Enable);
     CRC_Reset();
}
/********************************************************************
 *  END OF FILE:  CRC.c
********************************************************************/
