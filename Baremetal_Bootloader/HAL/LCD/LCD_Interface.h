/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  LCD_Interface.h
 *  Module:  HAL/LCD
 *  Description:  Liqued Crystal LCD Driver Interface File
*******************************************************************/
#ifndef _LCD_INTERFACE_H_
#define _LCD_INTERFACE_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "GPIO_Interface.h"
/*****************************************
--------    Type  Definitions    ---------
*****************************************/

/*****************************************
---------    Configure Macros    ---------
*****************************************/
#define Eight_Bit_Mode                   (1)
#define Four_Bit_Mode                    (2)
#define LCD_CLEAR_SCREEN				 (0x01)
#define LCD_FIRST_LINE			      (0x80)
#define LCD_SECOND_LINE		  		 (0xC0)
/*****************************************
---------   Prebuild Configure   ---------
*****************************************/
#define LCD_MODE                         (Four_Bit_Mode)
#define LCD_Data_Port                    (GPIO_A) 
#define LCD_Data_Pins                    {Pin_4,Pin_5,Pin_6,Pin_7} 
#define LCD_Control_Port                 (GPIO_A)
#define LCD_RS                           (Pin_10)
#define LCD_RW                           (Pin_11)
#define LCD_EN                           (Pin_12)
/*****************************************
---  Application Programming Interface  --
*****************************************/
/********************************************************************
* Syntax          : HAL_LCD_Send_Float(f32 Data)
* Description     : Send Float To LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy To u32 Float)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Send_Float(f32 Data);
/********************************************************************
* Syntax          : HAL_LCD_Send_Number(u32 Data)
* Description     : Send An Intiger To LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy To u32 INT)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Send_Number(u32 Data);
/********************************************************************
* Syntax          : HAL_LCD_Send_String(u8 String[])
* Description     : Send String To LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To String)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Send_String(char String[]);
/********************************************************************
* Syntax          : HAL_LCD_Set_Courser(u8 Y_Axis,u8 X_Axis)
* Description     : Set Courser For Specific Location
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy Row) (Copy Coloumn) 
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Set_Courser(u8 Y_Axis,u8 X_Axis);
/********************************************************************
* Syntax          : HAL_LCD_Clear()
* Description     : Clear Whole LCD 
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Clear(void);
/********************************************************************
* Syntax          : HAL_LCD_Initialization(void)
* Description     : Initialize LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Initialization(void);
/********************************************************************
* Syntax          : HAL_LCD_Send_Data(u8 Data)
* Description     : Send Data (Char) To LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy To The Charachter)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Send_Data(u8 Data);
/********************************************************************
 *  END OF FILE:  LCD_Interface.h
********************************************************************/
#endif
