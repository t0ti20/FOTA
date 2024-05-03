/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  EXTI_Interface.h
 *  Module:  MCAL/EXTI
 *  Description:  External Interrupt Device Driver Program File
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "EXTI_Interface.h"
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
static void (*volatile EXTI_IRQ_Call_Back_Function[15])(void)={NULL};
void EXTI_Initilization(EXTI_Config_t Configure)
{
     GPIO_Pin_Config_t EXTI_Pin={Pin_0,Alternate_Input,Input};
     u8 External_Interrupt_Map[16]=EXTI_Pins_Victor_Map;
     /*Enable GPIO And AFIO Clock*/
     RCC_Clock(RCC_Alternative,ENABLE);
     /*Configure GPIO As Input */
     EXTI_Pin.Pin_Number=Configure.Pin;
     MCAL_GPIO_Initialize(Configure.Port,EXTI_Pin);
     /*External Interrupt Configuration Register*/
     AFIO.EXTICRX[((Configure.Pin)/FOUR)].Register&=(u32)~((0xF)<<(((Configure.Pin)%FOUR)*FOUR));
     AFIO.EXTICRX[((Configure.Pin)/FOUR)].Register|=(u32)((Configure.Port)<<(((Configure.Pin)%FOUR)*FOUR));
     /*Rising Edge Configure*/
     switch (Configure.Sensing_Edge)
     {
          case Faling_Edge:Set_Bit(EXTI.FTSR.Register,Configure.Pin);Clear_Bit(EXTI.RTSR.Register,Configure.Pin);break;
          case Rising_Edge:Set_Bit(EXTI.RTSR.Register,Configure.Pin);Clear_Bit(EXTI.FTSR.Register,Configure.Pin);break;
          case Faling_Rising:Set_Bit(EXTI.RTSR.Register,Configure.Pin);Set_Bit(EXTI.FTSR.Register,Configure.Pin);break;
     }
     /*Configure Interrupts*/
     if(Configure.Interrupt==Enable)Set_Bit(EXTI.IMR.Register,Configure.Pin);
     else Clear_Bit(EXTI.IMR.Register,Configure.Pin);
     /*Configure Event*/
     if(Configure.Event==Enable)Set_Bit(EXTI.EMR.Register,Configure.Pin);
     else Clear_Bit(EXTI.EMR.Register,Configure.Pin);
     /*Configure Call Back Function*/
     EXTI_IRQ_Call_Back_Function[Configure.Pin]=Configure.Ptr_Function;
     /*Enable NVIC Mask*/
     NVIC_Interrupt(External_Interrupt_Map[Configure.Pin],Enable);
}
/********************************************************************
* Syntax          : MCAL_GPIO_Lock_Pin(GPIOX_t *Port,GPIO_Pin_t Pin)
* Description     : Lock State For Specific Pin
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(Copy To Pin Number "Pin_Number_t")
* Parameters (out): None
* Return value:   : Void
********************************************************************/
EXTI_IRQHandler(0)
{
     /*Execute ISR*/
     EXTI_IRQ_Call_Back_Function[0]();
     /*Clear Flag*/
     EXTI.PR.Bits.Bit_0=HI;
}
EXTI_IRQHandler(1)
{
     /*Execute ISR*/
     EXTI_IRQ_Call_Back_Function[1]();
     /*Clear Flag*/
     EXTI.PR.Bits.Bit_1=HI;
}
EXTI_IRQHandler(2)
{
     /*Execute ISR*/
     EXTI_IRQ_Call_Back_Function[2]();
     /*Clear Flag*/
     EXTI.PR.Bits.Bit_2=HI;
}
EXTI_IRQHandler(3)
{
     /*Execute ISR*/
     EXTI_IRQ_Call_Back_Function[3]();
     /*Clear Flag*/
     EXTI.PR.Bits.Bit_3=HI;
}
EXTI_IRQHandler(4)
{
     /*Execute ISR*/
     EXTI_IRQ_Call_Back_Function[4]();
     /*Clear Flag*/
     EXTI.PR.Bits.Bit_4=HI;
}
EXTI_IRQHandler(9_5)
{
     /*Get Interrupt Source And Clear Flag*/
     if(EXTI.PR.Bits.Bit_5){EXTI_IRQ_Call_Back_Function[5]();EXTI.PR.Bits.Bit_5=HI;}
     if(EXTI.PR.Bits.Bit_6){EXTI_IRQ_Call_Back_Function[6]();EXTI.PR.Bits.Bit_6=HI;}
     if(EXTI.PR.Bits.Bit_7){EXTI_IRQ_Call_Back_Function[7]();EXTI.PR.Bits.Bit_7=HI;}
     if(EXTI.PR.Bits.Bit_8){EXTI_IRQ_Call_Back_Function[8]();EXTI.PR.Bits.Bit_8=HI;}
     if(EXTI.PR.Bits.Bit_9){EXTI_IRQ_Call_Back_Function[9]();EXTI.PR.Bits.Bit_9=HI;}
}
EXTI_IRQHandler(15_10)
{
     /*Get Interrupt Source And Clear Flag*/
     if(EXTI.PR.Bits.Bit_10){EXTI_IRQ_Call_Back_Function[5]();EXTI.PR.Bits.Bit_10=HI;}
     if(EXTI.PR.Bits.Bit_12){EXTI_IRQ_Call_Back_Function[6]();EXTI.PR.Bits.Bit_12=HI;}
     if(EXTI.PR.Bits.Bit_13){EXTI_IRQ_Call_Back_Function[7]();EXTI.PR.Bits.Bit_13=HI;}
     if(EXTI.PR.Bits.Bit_14){EXTI_IRQ_Call_Back_Function[8]();EXTI.PR.Bits.Bit_14=HI;}
     if(EXTI.PR.Bits.Bit_15){EXTI_IRQ_Call_Back_Function[9]();EXTI.PR.Bits.Bit_15=HI;}
}
/********************************************************************
 *  END OF FILE:  EXTI_Program.c
********************************************************************/
