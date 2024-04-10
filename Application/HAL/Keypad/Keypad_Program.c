/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  Keypad_Program.c
 *  Module:  HAL/Keypad
 *  Description:  Keypad Driver Program File
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "Keypad_Interface.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/			                                                                   /*Initial Value For The Key*/
static u8 Keypad_Array[][4]=Keypad_Layout;
static u8 Keypad_Columns[]=Keypad_Columns_Pins;
static u8 Keypad_Rows[]=Keypad_Rows_Pins;
/********************************************************************
* Syntax          : u8 HAL_Keypad_Get_Key(void)
* Description     : Initialize Keypad With Pre Build Configurations
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value:   : Void
********************************************************************/
u8 HAL_Keypad_Wait_For_Input(void)
{
     u8 Input_Char;
     do Input_Char=HAL_Keypad_Get_Key();
     while(Input_Char==Keypad_Not_Pressed);
     return Input_Char;
}
/********************************************************************
* Syntax          : HAL_Keypad_Initialization(void)
* Description     : Initialize Keypad With Pre Build Configurations
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value:   : Void
********************************************************************/
u8 HAL_Keypad_Get_Key(void)
{
     u8 Column_Counter,Row_Counter,Keypad_Value=Keypad_Not_Pressed;
     for(Column_Counter=ZERO;Column_Counter<FOUR;Column_Counter++)
     {
          MCAL_GPIO_Write_Pin(Keypad_Port,Keypad_Columns[Column_Counter],LOW);
          for(Row_Counter=ZERO;Row_Counter<FOUR;Row_Counter++)
          {
               if(!MCAL_GPIO_Read_Pin(Keypad_Port,Keypad_Rows[Row_Counter]))
               {
                    Keypad_Value=Keypad_Array[Row_Counter][Column_Counter];
                    while (!MCAL_GPIO_Read_Pin(Keypad_Port,Keypad_Rows[Row_Counter]));
                    break;
               }
          }
          MCAL_GPIO_Write_Pin(Keypad_Port,Keypad_Columns[Column_Counter],HI);
     }
     return Keypad_Value;
}
/********************************************************************
* Syntax          : HAL_Keypad_Initialization(void)
* Description     : Initialize Keypad With Pre Build Configurations
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_Keypad_Initialization(void)
{
     GPIO_Pin_Number_t Pin_Counter;
     GPIO_Pin_Config_t Keypad_Pin;
     /*Configure Input Pins "Rows"*/
     Keypad_Pin.Pin_Mode=Input_Pull_Up;
     Keypad_Pin.Pin_Speed=Input;
     for(Pin_Counter=Pin_0;Pin_Counter<Pin_4;Pin_Counter++)
     {
          Keypad_Pin.Pin_Number=Keypad_Rows[Pin_Counter];
          MCAL_GPIO_Initialize(Keypad_Port,Keypad_Pin);
     }
     /*Configure Output Pins "Columns"*/
     Keypad_Pin.Pin_Mode=Output_Push_Pull;
     Keypad_Pin.Pin_Speed=Output_10;
     for(Pin_Counter=Pin_0;Pin_Counter<Pin_4;Pin_Counter++)
     {
          Keypad_Pin.Pin_Number=Keypad_Columns[Pin_Counter];
          MCAL_GPIO_Initialize(Keypad_Port,Keypad_Pin);
          MCAL_GPIO_Write_Pin(Keypad_Port,Keypad_Columns[Pin_Counter],HI);
     }
}
/********************************************************************
 *  END OF FILE:  Keypad_Program.c
********************************************************************/
