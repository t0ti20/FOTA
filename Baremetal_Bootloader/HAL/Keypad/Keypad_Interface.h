/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  Keypad_Interface.h
 *  Module:  HAL/Keypad
 *  Description:  Keypad Driver Interface File
*******************************************************************/
#ifndef _KEYPAD_INTERFACE_H_
#define _KEYPAD_INTERFACE_H_
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
#define Keypad_Not_Pressed		           ('X')
#define Keypad_FIRST_LINE			      (0x80)
#define Keypad_SECOND_LINE		  		 (0xC0)
/*****************************************
---------   Prebuild Configure   ---------
*****************************************/
/*Select Port To Connect Keypad "GPIOA - GPIOB - GPIOC - GPIOD"*/
#define Keypad_Port                          (GPIO_B)
/*Select Pins To Connect Keypad Rows*/
#define Keypad_Rows_Pins                     {Pin_0,Pin_1,Pin_2,Pin_3} 
/*Select Pins To Connect Keypad Columns*/
#define Keypad_Columns_Pins                  {Pin_4,Pin_5,Pin_6,Pin_7}
/*Select Keypad Layout*/
#define Keypad_Layout                        {{'7','8','9','/'},\
					                    {'4','5','6','*'}, \
					                    {'1','2','3','-'}, \
					                    {'O','0','=','+'}} \
/*****************************************
---  Application Programming Interface  --
*****************************************/
/********************************************************************
* Syntax          : HAL_Keypad_Initialization(void)
* Description     : Initialize Keypad With Pre Build Configurations
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_Keypad_Initialization(void);
u8 HAL_Keypad_Get_Key(void);
u8 HAL_Keypad_Wait_For_Input(void);
/********************************************************************
 *  END OF FILE:  Keypad_Interface.h
********************************************************************/
#endif
