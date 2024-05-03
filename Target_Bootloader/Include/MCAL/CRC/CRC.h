/*********************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  CRC.h
 *  Module:  MCAL/CRC
 *  Description:  CRC calculation unit Driver Interface File
*********************************************************************/
#ifndef _CRC_H_
#define _CRC_H_
/********************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "Macros.h"
/*****************************************
---  Application Programming Interface  --
*****************************************/
/********************************************************************
* Syntax          : CRC_Initialization(void)
* Description     : Initialize the CRC module for computations.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
********************************************************************/
void CRC_Initialization(void);
/********************************************************************
* Syntax          : CRC_Reset(void)
* Description     : Resets the CRC computation to its initial state.
* Sync-Async      : Synchronous
* Reentrancy      : Non-Reentrant
* Parameters (in) : None
* Parameters (out): None
* Return value    : Void
********************************************************************/
void CRC_Reset(void);
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
u32 CRC_Accumulate(u32 Value);
/********************************************************************
 *  END OF FILE:  CRC.h
********************************************************************/
#endif
