/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  LCD_Program.c
 *  Module:  HAL/LCD
 *  Description:  Liqued Crystal LCD Driver Program File
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "LCD_Interface.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/********************************************************************
* Syntax          : HAL_LCD_Send_Command(u8 Command)
* Description     : Send Command To LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy To The Command)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
static void HAL_LCD_Send_Command(u8 Command)
{
#if LCD_MODE == Eight_Bit_Mode
     GPIO_Pin_Number_t Pin_Counter,Pins[EIGHT]=LCD_Data_Pins;
#elif LCD_MODE == Four_Bit_Mode
     GPIO_Pin_Number_t Pin_Counter,Pins[FOUR]=LCD_Data_Pins;
#endif
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_RW,LOW);           /*Turn RW Off*/
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_RS,LOW);           /*Turn RS Off*/
#if LCD_MODE == Eight_Bit_Mode
     /*Write Command*/
     for(Pin_Counter=Pin_0;Pin_Counter<Pin_8;Pin_Counter++)MCAL_GPIO_Write_Pin(LCD_Data_Port,Pins[Pin_Counter],((Command>>Pin_Counter)&ONE));
#elif LCD_MODE == Four_Bit_Mode
     /*Send Hi Nibble*/
     for(Pin_Counter=Pin_0;Pin_Counter<Pin_4;Pin_Counter++)MCAL_GPIO_Write_Pin(LCD_Data_Port,Pins[Pin_Counter],((Command>>(Pin_Counter+FOUR))&ONE));
     /*Enable LCD*/
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_EN,HI);            /*Wait For LCD*/
     delay_ms(5)                                                 /*Wait For LCD*/
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_EN,LOW);           /*Wait For LCD*/
     delay_ms(2)
     /*Send Low Nibble*/ 
     for(Pin_Counter=Pin_0;Pin_Counter<Pin_4;Pin_Counter++)MCAL_GPIO_Write_Pin(LCD_Data_Port,Pins[Pin_Counter],((Command>>Pin_Counter)&ONE));
#endif 
     /*Enable LCD*/
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_EN,HI);            /*Wait For LCD*/
     delay_ms(5)                                                 /*Wait For LCD*/
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_EN,LOW);           /*Wait For LCD*/
     delay_ms(2)                                                 /*Wait For LCD*/
}
/********************************************************************
* Syntax          : HAL_LCD_Initialization(void)
* Description     : Initialize LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Initialization(void)
{
#if LCD_MODE == Eight_Bit_Mode
     GPIO_Pin_Number_t Pin_Counter,Pins[EIGHT]=LCD_Data_Pins;
#elif LCD_MODE == Four_Bit_Mode
     GPIO_Pin_Number_t Pin_Counter,Pins[FOUR]=LCD_Data_Pins;
#endif
     GPIO_Pin_Config_t LCD_Pin;
     /*Initialize Control Pins*/
     LCD_Pin.Pin_Mode=Output_Push_Pull;
     LCD_Pin.Pin_Speed=Output_10;
     LCD_Pin.Pin_Number=LCD_RS;
     MCAL_GPIO_Initialize(LCD_Control_Port,LCD_Pin);
     LCD_Pin.Pin_Number=LCD_RW;
     MCAL_GPIO_Initialize(LCD_Control_Port,LCD_Pin);
     LCD_Pin.Pin_Number=LCD_EN;
     MCAL_GPIO_Initialize(LCD_Control_Port,LCD_Pin);
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_EN,LOW);
#if LCD_MODE == Eight_Bit_Mode
     /*Initialize Data Pins*/
     for(Pin_Counter=Pin_0;Pin_Counter<Pin_8;Pin_Counter++)
     {
          LCD_Pin.Pin_Number=Pins[Pin_Counter];
          MCAL_GPIO_Initialize(LCD_Data_Port,LCD_Pin);
     }
     delay_ms(40)
     /* Configure LCD */
     HAL_LCD_Send_Command(0x3C);
     HAL_LCD_Send_Command(0x0C);
     HAL_LCD_Send_Command(HI);
#elif LCD_MODE == Four_Bit_Mode
     /*Initialize Data Pins*/
     for(Pin_Counter=Pin_0;Pin_Counter<Pin_4;Pin_Counter++)
     {
          LCD_Pin.Pin_Number=Pins[Pin_Counter];
          MCAL_GPIO_Initialize(LCD_Data_Port,LCD_Pin);
     }
     delay_ms(40)
     /* Configure LCD */
     HAL_LCD_Send_Command(0X02);
     HAL_LCD_Send_Command(0X28);
     HAL_LCD_Send_Command(0X0C);
     HAL_LCD_Send_Command(0X06);
#else
#warning (" Wrong Configuration !!")
#endif
}
/********************************************************************
* Syntax          : HAL_LCD_Send_Data(u8 Data)
* Description     : Send Data (Char) To LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy To The Charachter)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Send_Data(u8 Data)
{
#if LCD_MODE == Eight_Bit_Mode
     GPIO_Pin_Number_t Pin_Counter,Pins[EIGHT]=LCD_Data_Pins;
#elif LCD_MODE == Four_Bit_Mode
     GPIO_Pin_Number_t Pin_Counter,Pins[FOUR]=LCD_Data_Pins;
#endif
     delay_ms(50) 
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_RW,LOW);           /*Turn RW Off*/
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_RS,HI);            /*Turn RS HI*/
#if LCD_MODE == Eight_Bit_Mode
     /*Write Data*/
     for(Pin_Counter=Pin_0;Pin_Counter<Pin_8;Pin_Counter++)
          MCAL_GPIO_Write_Pin(LCD_Data_Port,Pins[Pin_Counter],((Data>>Pin_Counter)&ONE));
#elif LCD_MODE == Four_Bit_Mode
     /*Send Hi Nibble*/
     for(Pin_Counter=Pin_0;Pin_Counter<Pin_4;Pin_Counter++)MCAL_GPIO_Write_Pin(LCD_Data_Port,Pins[Pin_Counter],((Data>>(Pin_Counter+FOUR))&ONE));
     /*Enable LCD*/
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_EN,HI);            /*Wait For LCD*/
     delay_ms(5)                                                 /*Wait For LCD*/
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_EN,LOW);           /*Wait For LCD*/
     delay_ms(2) 
     /*Send Low Nibble*/  
     for(Pin_Counter=Pin_0;Pin_Counter<Pin_4;Pin_Counter++)MCAL_GPIO_Write_Pin(LCD_Data_Port,Pins[Pin_Counter],((Data>>Pin_Counter)&ONE));
#endif 
     /*Enable LCD*/
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_EN,HI);            /*Wait For LCD*/
     delay_ms(5)                                                 /*Wait For LCD*/
     MCAL_GPIO_Write_Pin(LCD_Control_Port,LCD_EN,LOW);           /*Wait For LCD*/
     delay_ms(2)                                                 /*Wait For LCD*/
}
/********************************************************************
* Syntax          : itoa(u32 Data,u8 String[])
* Description     : Convert Integer To String
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy To Integer) (Ptr To String)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
static void itoa(u32 Data,char String[])
{
     u8 Digit=ZERO,Count;
     if(!Data)String[Digit++]='0';
     else for(;Data;Digit++,Data/=TEN)String[Digit]=(Data%TEN+'0');
     String[Digit]=ZERO;
     for(Count=ZERO;Count<(Digit>>ONE);Count++)
     {
          String[Count]^=String[Digit-Count-ONE];
          String[Digit-Count-ONE]^=String[Count];
          String[Count]^=String[Digit-Count-ONE];
     }
}
/********************************************************************
* Syntax          : ftoa(f32 Data,u8 String[])
* Description     : Convert Float To String
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy To Float) (Ptr To String)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
static void ftoa(f32 Data,char String[])
{
     u8 Digit,Count;
     u16 Real=(u16)(Data-(u16)Data)*100;
     for(Digit=ZERO;(u32)Real;Digit++,Real/=TEN)String[Digit]=((u32)Real%TEN+'0');
     String[Digit++]='.';
     for(;(u32)Data;Digit++,Data/=TEN)String[Digit]=((u32)Data%TEN+'0');
     for(Count=ZERO;Count<(Digit>>ONE);Count++)
     {
          String[Count]^=String[Digit-Count-ONE];
          String[Digit-Count-ONE]^=String[Count];
          String[Count]^=String[Digit-Count-ONE];
     }
}
/********************************************************************
* Syntax          : HAL_LCD_Send_Float(f32 Data)
* Description     : Send Float To LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy To u32 Float)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Send_Float(f32 Data)
{
     char String[8];
     ftoa(Data,String);
     HAL_LCD_Send_String(String);
}
/********************************************************************
* Syntax          : HAL_LCD_Send_Number(u32 Data)
* Description     : Send An Intiger To LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy To u32 INT)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Send_Number(u32 Data)
{
     char String[8];
     itoa(Data,String);
     HAL_LCD_Send_String(String);
}
/********************************************************************
* Syntax          : HAL_LCD_Send_String(u8 String[])
* Description     : Send String To LCD
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To String)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Send_String(char String[])
{
     while(*String)
     {
          HAL_LCD_Send_Data(*String);
          String++;
     }
}
/********************************************************************
* Syntax          : HAL_LCD_Set_Courser(u8 Y_Axis,u8 X_Axis)
* Description     : Set Courser For Specific Location
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Copy Row) (Copy Coloumn) 
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Set_Courser(u8 Y_Axis,u8 X_Axis)
{
     u8 Line_Select=Y_Axis?LCD_SECOND_LINE:LCD_FIRST_LINE;
     HAL_LCD_Send_Command(Line_Select+X_Axis);
}
/********************************************************************
* Syntax          : HAL_LCD_Clear()
* Description     : Clear Whole LCD 
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void HAL_LCD_Clear(void)
{
     HAL_LCD_Send_Command(LCD_CLEAR_SCREEN);
}
/********************************************************************
 *  END OF FILE:  LCD_Program.c
********************************************************************/
