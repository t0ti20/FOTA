/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  RCC_Program.c
 *  Module:  MCAL/RCC
 *  Description:  Reset And Clock Control Driver Program File For STM32F103C6
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "RCC_Interface.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/********************************************************************
* Syntax          : static u64 RCC_Get_PLL2_Clock(void)
* Description     : Get PLL2 Clock
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Void
* Parameters (out): None
* Return value:   : u64 Frequency in HZ
********************************************************************/
static u64 RCC_Get_PLL2_Clock(void)
{
     u64 Clock_Frequency=HSE_OSC;
     /*PREDIV2*/
     Clock_Frequency/=(Get_Bits(RCC.CFGR2.Register,FOUR,FOUR)+ONE);
     /*PLL2MUL*/
     switch (Get_Bits(RCC.CFGR2.Register,EIGHT,FOUR))
     {
          case RCC_PLL2_X9:Clock_Frequency*=9;break;
          case RCC_PLL2_X10:Clock_Frequency*=10;break;
          case RCC_PLL2_X11:Clock_Frequency*=11;break;
          case RCC_PLL2_X12:Clock_Frequency*=12;break;
          case RCC_PLL2_X13:Clock_Frequency*=13;break;
          case RCC_PLL2_X14:Clock_Frequency*=14;break;
          case RCC_PLL2_X16:Clock_Frequency*=16;break;
          case RCC_PLL2_X20:Clock_Frequency*=20;break;
          default:break;
     }
     return Clock_Frequency;
}
/********************************************************************
* Syntax          : static u64 RCC_Get_PLL2_Clock(void)
* Description     : Get PLL1 Clock
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Void
* Parameters (out): None
* Return value:   : u64 Frequency in HZ
********************************************************************/
static u64 RCC_Get_PLL_Clock(void)
{
     u64 Clock_Frequency=ZERO;
     /*Clock from PREDIV1 selected as PLL input clock*/
     if(RCC.CFGR.Bits.Bit_16)
     {
          /*PLL2 selected as PREDIV1 clock entry*/
          if(RCC.CFGR2.Bits.Bit_16)Clock_Frequency=RCC_Get_PLL2_Clock();
          /*HSE oscillator clock selected as PREDIV1 clock entry*/
          else Clock_Frequency=HSE_OSC;
          Clock_Frequency/=(Get_Bits(RCC.CFGR2.Register,ZERO,FOUR)+ONE);
     }
     /*HSI oscillator clock / 2 selected as PLL input clock*/
     else Clock_Frequency=(4000000UL);                
     /*PLLMUL*/
     switch (Get_Bits(RCC.CFGR.Register,18,FOUR))
     {
          case RCC_PLL1_X4:Clock_Frequency*=FOUR;break;
          case RCC_PLL1_X5:Clock_Frequency*=FIVE;break;
          case RCC_PLL1_X6:Clock_Frequency*=SIX;break;
          case RCC_PLL1_X7:Clock_Frequency*=SEVEN;break;
          case RCC_PLL1_X8:Clock_Frequency*=EIGHT;break;
          case RCC_PLL1_X9:Clock_Frequency*=NINE;break;
          case RCC_PLL1_X6_5:Clock_Frequency*=65;Clock_Frequency/=10;break;
          default:break;
     }
     return Clock_Frequency;
}
/********************************************************************
* Syntax          : void RCC_Clock(RCC_Modules_t RCC_Module,logic_t State)
* Description     : Enable And Disable Clock For Specific Module
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (RCC_Modules_t Module Name),(logic_t State)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void RCC_Clock(RCC_Modules_t RCC_Module,logic_t State)
{
     if(State==ENABLE)
     {
          switch (RCC_Module/32)
          {
               case AHBENR_Base:Set_Bit(RCC.AHBENR.Register,RCC_Module%32);break;
               case APB2ENR_Base:Set_Bit(RCC.APB2ENR.Register,RCC_Module%32);break;
               case APB1ENR_Base:Set_Bit(RCC.APB1ENR.Register,RCC_Module%32);break;
               default:break;
          }
     }
     else
     {
          switch (RCC_Module/32)
          {
               case AHBENR_Base:Clear_Bit(RCC.AHBENR.Register,RCC_Module%32);break;
               case APB2ENR_Base:Clear_Bit(RCC.APB2ENR.Register,RCC_Module%32);break;
               case APB1ENR_Base:Clear_Bit(RCC.APB1ENR.Register,RCC_Module%32);break;
               default:break;
          }
     }
}
/********************************************************************
* Syntax          : u64 RCC_Get_System_Clock(void)
* Description     : Get System Clock Frequency
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Void
* Parameters (out): None
* Return value:   : u64 Frequency in HZ
********************************************************************/
u64 RCC_Get_System_Clock(void)
{
     u64 Clock_Frequency=ZERO;
     switch (Get_Bits(RCC.CFGR.Register,ZERO,TWO))
     {
          case RCC_HSI:Clock_Frequency=HSI_RC;break;
          case RCC_HSE:Clock_Frequency=HSE_OSC;break;
          case RCC_PLL:Clock_Frequency=RCC_Get_PLL_Clock();break;
          default:break;
     }
     return Clock_Frequency;
}
/********************************************************************
* Syntax          : u64 RCC_Get_System_Clock(void)
* Description     : Get Advanced Hi Performance Bus Clock Frequency
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Void
* Parameters (out): None
* Return value:   : u64 Frequency in HZ
********************************************************************/
u64 RCC_Get_AHB_Clock(void)
{
     u64 Clock_Frequency=RCC_Get_System_Clock();
     if(RCC.CFGR.Bits.Bit_7)Clock_Frequency=(u64)(((u64)Clock_Frequency)>>(Get_Bits(RCC.CFGR.Register,FOUR,THREE)+ONE));
     return Clock_Frequency;
}
/********************************************************************
* Syntax          : u64 RCC_Get_System_Clock(void)
* Description     : Advanced Peripheral Bus 1 Clock Frequency
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Void
* Parameters (out): None
* Return value:   : u64 Frequency in HZ
********************************************************************/
u64 RCC_Get_APB1_Clock(void)
{
     u64 Clock_Frequency=RCC_Get_AHB_Clock();
     if(RCC.CFGR.Bits.Bit_10)Clock_Frequency=(u64)(((u64)Clock_Frequency)>>(Get_Bits(RCC.CFGR.Register,EIGHT,TWO)+ONE));
     return Clock_Frequency;
}
/********************************************************************
* Syntax          : u64 RCC_Get_System_Clock(void)
* Description     : Advanced Peripheral Bus 2 Clock Frequency
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Void
* Parameters (out): None
* Return value:   : u64 Frequency in HZ
********************************************************************/
u64 RCC_Get_APB2_Clock(void)
{
     u64 Clock_Frequency=RCC_Get_AHB_Clock();
     if(RCC.CFGR.Bits.Bit_13)Clock_Frequency=(u64)(((u64)Clock_Frequency)>>(Get_Bits(RCC.CFGR.Register,11,TWO)+ONE));
     return Clock_Frequency;
}
/********************************************************************
 *  END OF FILE:  RCC_Program.c
********************************************************************/
