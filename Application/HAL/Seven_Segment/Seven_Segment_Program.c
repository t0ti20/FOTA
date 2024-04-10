/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  Seven_Segment_Program.c
 *  Module:  HAL/Seven_Segment
 *  Description:  Seven Segment Program File
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "Seven_Segment_Interface.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
static u8 Seven_Segment_Array[TEN]=Seven_Segment_Cathode_Array;
/********************************************************************
* Syntax          : HAL_LCD_Send_Command(u8 Command)
* Description     : Send Command To LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy To The Command)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void Seven_Segment_Initialization(Seven_Segment_t *Seven_Segment)
{
     GPIO_Pin_Number_t Pin_Counter;
     GPIO_Pin_Config_t Seven_Segment_Pin;
     /*Initialize Control Pin*/
     Seven_Segment_Pin.Pin_Mode=Output_Push_Pull;
     Seven_Segment_Pin.Pin_Speed=Output_10;
     Seven_Segment_Pin.Pin_Number=Seven_Segment->Enable_Pin;
     MCAL_GPIO_Initialize(Seven_Segment->Port,Seven_Segment_Pin);
     /*Initialize Data Pins*/
     for(Pin_Counter=Pin_0;Pin_Counter<Pin_8;Pin_Counter++)
     {
          Seven_Segment_Pin.Pin_Number=Seven_Segment->Pin_Numbers[Pin_Counter];
          MCAL_GPIO_Initialize(Seven_Segment->Port,Seven_Segment_Pin);
     }
}
void Seven_Segment_Send_Two_Digits(Seven_Segment_t *Seven_Segment_One,Seven_Segment_t *Seven_Segment_Two,u8 Digit)
{
     if(Seven_Segment_One->Pin_Numbers[0]==Seven_Segment_Two->Pin_Numbers[0]&&Seven_Segment_One->Port==Seven_Segment_Two->Port)
     {
          Seven_Segment_Send_One_Digit(Seven_Segment_One,Digit%TEN);
          MCAL_GPIO_Write_Pin(Seven_Segment_One->Port,Seven_Segment_One->Enable_Pin,Seven_Segment_One->Seven_Segment_Mode==Common_Cathode?HI:LOW);
          Seven_Segment_Send_One_Digit(Seven_Segment_Two,Digit/TEN);
          MCAL_GPIO_Write_Pin(Seven_Segment_Two->Port,Seven_Segment_Two->Enable_Pin,Seven_Segment_Two->Seven_Segment_Mode==Common_Cathode?HI:LOW);
     }
     else
     {
          Seven_Segment_Send_One_Digit(Seven_Segment_One,Digit%TEN);
          Seven_Segment_Send_One_Digit(Seven_Segment_Two,Digit/TEN);
     }
}
void Seven_Segment_Send_One_Digit(Seven_Segment_t *Seven_Segment,u8 Digit)
{
     GPIO_Pin_Number_t Pin_Counter;
     /*Common Cathode Configuration*/
     if(Seven_Segment->Seven_Segment_Mode)
     {
          MCAL_GPIO_Write_Pin(Seven_Segment->Port,Seven_Segment->Enable_Pin,LOW);
          for(Pin_Counter=Pin_0;Pin_Counter<Pin_8;Pin_Counter++)
          {
               MCAL_GPIO_Write_Pin(Seven_Segment->Port,Seven_Segment->Pin_Numbers[Pin_Counter],Get_Bit(Seven_Segment_Array[Digit],Pin_Counter));
          }
     }
     /*Common Anode Configuration*/
     else
     {
          MCAL_GPIO_Write_Pin(Seven_Segment->Port,Seven_Segment->Enable_Pin,HI);
          for(Pin_Counter=Pin_0;Pin_Counter<Pin_8;Pin_Counter++)
          {
               MCAL_GPIO_Write_Pin(Seven_Segment->Port,Seven_Segment->Pin_Numbers[Pin_Counter],!Get_Bit(Seven_Segment_Array[Digit],Pin_Counter));
          }
     }
}
/********************************************************************
 *  END OF FILE:  Seven_Segment_Program.c
********************************************************************/
