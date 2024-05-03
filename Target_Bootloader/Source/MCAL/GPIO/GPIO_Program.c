/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  GPIO_Program.c
 *  Module:  MCAL/GPIO
 *  Description:  General Input Output Driver Program File For STM32F103C6
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "GPIO_Interface.h"
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
void MCAL_GPIO_Lock_Pin(GPIO_Port_t Port_Number,GPIO_Pin_Number_t Pin)
{
     GPIOX_t *Port=NULL;
	 RCC.APB1ENR.Bits.Bit_0=1;
     switch (Port_Number)
     {
          case GPIO_A:Port=&GPIOA;break;
          case GPIO_B:Port=&GPIOB;break;
          case GPIO_C:Port=&GPIOC;break;
          case GPIO_D:Port=&GPIOD;break;
     }
     Port->LCKR.Register=(((u32)ONE<<Pin)|(u32)(0x10000));
     Port->LCKR.Bits.Bit_16=HI;
     Port->LCKR.Bits.Bit_16=LOW;
     Port->LCKR.Bits.Bit_16=HI;
     Port->LCKR.Bits.Bit_16;
	Port->LCKR.Bits.Bit_16;
}
/********************************************************************
* Syntax          : MCAL_GPIO_Initialize(GPIOX_t *Port,GPIO_Pin_t Pin)
* Description     : Write Logic On Specific Pin
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(Pin Configuration "GPIO_Pin_t")
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void MCAL_GPIO_Initialize(GPIO_Port_t Port_Number,GPIO_Pin_Config_t Pin)
{
     GPIOX_t *Port=NULL;
	Register_t volatile *Configure_Register=NULL;
     switch (Port_Number)
     {
          case GPIO_A:RCC_Clock(RCC_GPIOA,Enable);Port=&GPIOA;break;
          case GPIO_B:RCC_Clock(RCC_GPIOB,Enable);Port=&GPIOB;break;
          case GPIO_C:RCC_Clock(RCC_GPIOC,Enable);Port=&GPIOC;break;
          case GPIO_D:RCC_Clock(RCC_GPIOD,Enable);Port=&GPIOD;break;
     }
     if(Pin.Pin_Number<Pin_8) Configure_Register=&(Port->CRL);
     else
     {
          Pin.Pin_Number-=EIGHT;
          Configure_Register=&(Port->CRH);
     }
     if(Pin.Pin_Mode==Input_Pull_Down)
     {
          Clear_Bit(Port->ODR.Register,Pin.Pin_Number);
          Pin.Pin_Mode=Input_Pull_Up;
     }
     else if (Pin.Pin_Mode==Input_Pull_Up) Set_Bit(Port->ODR.Register,Pin.Pin_Number);
     Configure_Register->Register&=(~((u32)0XF<<(Pin.Pin_Number*FOUR)));
     Configure_Register->Register|=((((u32)Pin.Pin_Speed|(u32)Pin.Pin_Mode))<<(Pin.Pin_Number*FOUR));
}
/********************************************************************
* Syntax          : MCAL_GPIO_Read_Pin(GPIOX_t *Port,GPIO_Pin_t Pin)
* Description     : Read Logic State For Specific Pin
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(Copy To Pin Number "Pin_Number_t")
* Parameters (out): None
* Return value:   : u8 Refers To Pin State "HI-LOW"
********************************************************************/
u8 MCAL_GPIO_Read_Pin(GPIO_Port_t Port_Number,GPIO_Pin_Number_t Pin)
{
     GPIOX_t *Port=NULL;
     switch (Port_Number)
     {
          case GPIO_A:Port=&GPIOA;break;
          case GPIO_B:Port=&GPIOB;break;
          case GPIO_C:Port=&GPIOC;break;
          case GPIO_D:Port=&GPIOD;break;
     }
     return (u8)Get_Bit(Port->IDR.Register,Pin);
}
/********************************************************************
* Syntax          : MCAL_GPIO_Write_Pin(GPIOX_t *Port,GPIO_Pin_t Pin)
* Description     : Write Logic On Specific Pin
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(Copy To Pin Number "GPIO_Pin_Number_t"),(State "HI-LOW")
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void MCAL_GPIO_Write_Pin(GPIO_Port_t Port_Number,GPIO_Pin_Number_t Pin,u8 State)
{
     GPIOX_t *Port=NULL;
     switch (Port_Number)
     {
          case GPIO_A:Port=&GPIOA;break;
          case GPIO_B:Port=&GPIOB;break;
          case GPIO_C:Port=&GPIOC;break;
          case GPIO_D:Port=&GPIOD;break;
     }
     Port->BSRR.Register=(State==HI)?(u32)(ONE<<Pin):(u32)(ONE<<(Pin+16));
}
/********************************************************************
* Syntax          : MCAL_GPIO_Toggle_Pin(GPIOX_t *Port,GPIO_Pin_t Pin)
* Description     : Toggle Specific Pin
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(Copy To Pin Number "Pin_Number_t")
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void MCAL_GPIO_Toggle_Pin(GPIO_Port_t Port_Number,GPIO_Pin_Number_t Pin)
{
     GPIOX_t *Port=NULL;
     switch (Port_Number)
     {
          case GPIO_A:Port=&GPIOA;break;
          case GPIO_B:Port=&GPIOB;break;
          case GPIO_C:Port=&GPIOC;break;
          case GPIO_D:Port=&GPIOD;break;
     }
     Toggle_Bit(Port->ODR.Register,Pin);
}
/********************************************************************
* Syntax          : MCAL_GPIO_Read_Port(GPIOX_t *Port)
* Description     : Read Logic State For Specific Port
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),
* Parameters (out): None
* Return value:   : u16 Refers To Port Value
********************************************************************/
u16 MCAL_GPIO_Read_Port(GPIO_Port_t Port_Number)
{
     GPIOX_t *Port=NULL;
     switch (Port_Number)
     {
          case GPIO_A:Port=&GPIOA;break;
          case GPIO_B:Port=&GPIOB;break;
          case GPIO_C:Port=&GPIOC;break;
          case GPIO_D:Port=&GPIOD;break;
     }
     return (u16)Port->IDR.Register;
}
/********************************************************************
* Syntax          : MCAL_GPIO_Write_Port(GPIOX_t *Port)
* Description     : Write Logic On Specific Port
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(u16 Value)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void MCAL_GPIO_Write_Port(GPIO_Port_t Port_Number,u16 Value)
{
     GPIOX_t *Port=NULL;
     switch (Port_Number)
     {
          case GPIO_A:Port=&GPIOA;break;
          case GPIO_B:Port=&GPIOB;break;
          case GPIO_C:Port=&GPIOC;break;
          case GPIO_D:Port=&GPIOD;break;
     }
     Port->ODR.Register=(u16)Value;
}
/********************************************************************
* Syntax          : MCAL_GPIO_Toggle_Port(GPIOX_t *Port)
* Description     : Toggle All Pins On Specific Port
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void MCAL_GPIO_Toggle_Port(GPIO_Port_t Port_Number)
{
     GPIOX_t *Port=NULL;
     switch (Port_Number)
     {
          case GPIO_A:Port=&GPIOA;break;
          case GPIO_B:Port=&GPIOB;break;
          case GPIO_C:Port=&GPIOC;break;
          case GPIO_D:Port=&GPIOD;break;
     }
     Port->ODR.Register^=0xFFFFFFFF;
}
/********************************************************************
 *  END OF FILE:  GPIO_Program.c
********************************************************************/
