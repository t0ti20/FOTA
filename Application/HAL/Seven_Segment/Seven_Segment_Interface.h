/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  Seven_Segment_Interface.h
 *  Module:  HAL/LCD
 *  Description:  Seven Segment Interface File
*******************************************************************/
#ifndef _SEVEN_SEGMENT_INTERFACE_H_
#define _SEVEN_SEGMENT_INTERFACE_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "GPIO_Interface.h"
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
typedef enum Seven_Segment_Mode_t{Common_Anode,Common_Cathode}Seven_Segment_Mode_t;
#pragma pack(1)
/*Port,Seven_Segment_Mode,{Pin_Numbers},Enable_Pin*/
typedef struct Seven_Segment_t
{
	GPIO_Port_t Port;
	Seven_Segment_Mode_t Seven_Segment_Mode;
     GPIO_Pin_Number_t Pin_Numbers[EIGHT];
	GPIO_Pin_Number_t Enable_Pin;
}Seven_Segment_t;

#pragma pack()
/*****************************************
---------    Configure Macros    ---------
*****************************************/
/*Common Cathode 7-Segment*/
#define Seven_Segment_Cathode_Array     {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F}
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
void Seven_Segment_Initialization(Seven_Segment_t *Seven_Segment);
void Seven_Segment_Send_One_Digit(Seven_Segment_t *Seven_Segment,u8 Digit);
void Seven_Segment_Send_Two_Digits(Seven_Segment_t *Seven_Segment_One,Seven_Segment_t *Seven_Segment_Two,u8 Digit);
/********************************************************************
 *  END OF FILE:  Seven_Segment_Interface.h
********************************************************************/
#endif
