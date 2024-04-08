/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  SPI_Program.c
 *  Module:  MCAL/SPI
 *  Description:  Reset And Clock Control Driver Program File For STM32F103C6
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "SPI_Interface.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/********************************************************************
* Syntax          : static u64 SPI_Get_PLL2_Clock(void)
* Description     : Get PLL2 Clock
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Void
* Parameters (out): None
* Return value:   : u64 Frequency in HZ
********************************************************************/
void SPI_Send_Receive_Data(SPI_Config_t *SPI_Config,u16 *Data)
{
     SPI_t *SPIX;
     /*Select SPI Number*/
     switch (SPI_Config->SPI_Number)
     {
          case SPI_1:SPIX=&SPI1;break;
          case SPI_2:SPIX=&SPI2;break;
          case SPI_3:SPIX=&SPI3;break;
     }
     /*Send Data*/
     /*Check For Interrupt*/
     if(SPI_Config->Interrupt==SPI_Disable_Interrupt){while (!SPIX->SR.Bits.Bit_1);}
     else {}
     /*Write Data*/
     SPIX->DR.Register=*Data;
     /*Receive Data*/
     if(SPI_Config->Interrupt==SPI_Disable_Interrupt){while (!SPIX->SR.Bits.Bit_0);}
     else {}
     /*Write Data*/
     *Data=(u16)SPIX->DR.Register;
}
/********************************************************************
* Syntax          : static u64 SPI_Get_PLL2_Clock(void)
* Description     : Get PLL2 Clock
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Void
* Parameters (out): None
* Return value:   : u64 Frequency in HZ
********************************************************************/
void SPI_Send_Data(SPI_Config_t *SPI_Config,u16 Data)
{
     SPI_t *SPIX;
     /*Select SPI Number*/
     switch (SPI_Config->SPI_Number)
     {
          case SPI_1:SPIX=&SPI1;break;
          case SPI_2:SPIX=&SPI2;break;
          case SPI_3:SPIX=&SPI3;break;
     }
     /*Busy Wait Till Buffer Empty*/
     while (!SPIX->SR.Bits.Bit_1);
     /*Write Data*/
     SPIX->DR.Register=Data;
}
/********************************************************************
* Syntax          : static u64 SPI_Get_PLL2_Clock(void)
* Description     : Get PLL2 Clock
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Void
* Parameters (out): None
* Return value:   : u64 Frequency in HZ
********************************************************************/
void SPI_Receive_Data(SPI_Config_t *SPI_Config,u16 *Data)
{
     SPI_t *SPIX;
     /*Select SPI Number*/
     switch (SPI_Config->SPI_Number)
     {
          case SPI_1:SPIX=&SPI1;break;
          case SPI_2:SPIX=&SPI2;break;
          case SPI_3:SPIX=&SPI3;break;
     }
     /*Busy Wait Till Buffer Empty*/
     while (!SPIX->SR.Bits.Bit_0);
     /*Read Data*/
     *Data=(u16)SPIX->DR.Register;
}
/********************************************************************
* Syntax          : static u64 SPI_Get_PLL2_Clock(void)
* Description     : Get PLL2 Clock
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Void
* Parameters (out): None
* Return value:   : u64 Frequency in HZ
********************************************************************/
static void (*volatile SPI_IRQ_Call_Back_Function[3])(void)={NULL,NULL,NULL};
void SPI_Initialization(SPI_Config_t *SPI_Config)
{
     SPI_t *SPIX;
	GPIO_Pin_Config_t Pin;
     GPIO_Pin_Number_t NSS,SCK,MISO,MOSI;
     GPIO_Port_t Port;
     Register_t Local_CR1,Local_CR2;Local_CR1.Register=ZERO;Local_CR2.Register=ZERO;
     /*Select SPI Number And Enable Clock*/
     switch (SPI_Config->SPI_Number)
     {
          case SPI_1:
          Port=GPIO_A;NSS=Pin_4;SCK=Pin_5;MISO=Pin_6;MOSI=Pin_7;
          SPI_IRQ_Call_Back_Function[0]=SPI_Config->Call_Back_Function;
          SPIX=&SPI1;RCC_Clock(RCC_SPI1,Enable);
          break;
          case SPI_2:
          Port=GPIO_B;NSS=Pin_12;SCK=Pin_13;MISO=Pin_14;MOSI=Pin_15;
          SPI_IRQ_Call_Back_Function[1]=SPI_Config->Call_Back_Function;
          SPIX=&SPI2;RCC_Clock(RCC_SPI2,Enable);
          break;
          case SPI_3:
          Port=GPIO_B;NSS=Pin_12;SCK=Pin_3;MISO=Pin_4;MOSI=Pin_5;
          SPI_IRQ_Call_Back_Function[2]=SPI_Config->Call_Back_Function;
          SPIX=&SPI3;RCC_Clock(RCC_SPI3,Enable);
          break;
     }
     /*NSS Pin*/
     if(SPI_Config->Slave_Select==SPI_Hardware_Slave)
     {
          Pin.Pin_Speed=Input;
          Pin.Pin_Mode=Input_Floating;
          Pin.Pin_Number=NSS;
          MCAL_GPIO_Initialize(Port,Pin);
     }
     else if(SPI_Config->Slave_Select==SPI_Hardware_Master_Tx)
     {
          Pin.Pin_Speed=Output_10;
          Pin.Pin_Mode=Output_Push_Pull;
          Pin.Pin_Number=NSS;
          MCAL_GPIO_Initialize(Port,Pin);
     }
     /*SCK Pin*/
     if(SPI_Config->Mode==SPI_Master)
     {
          Pin.Pin_Speed=Output_10;
          Pin.Pin_Mode=Alternate_Output_Push_Pull;
          Pin.Pin_Number=SCK;
          MCAL_GPIO_Initialize(Port,Pin);
     }
     else
     {
          Pin.Pin_Speed=Input;
          Pin.Pin_Mode=Input_Floating;
          Pin.Pin_Number=SCK;
          MCAL_GPIO_Initialize(Port,Pin);
     }
     /*MISO Pin Only Full duplex*/
     if(SPI_Config->Mode==SPI_Master)
     {
          Pin.Pin_Speed=Input;
          Pin.Pin_Mode=Input_Floating;
          Pin.Pin_Number=MISO;
          MCAL_GPIO_Initialize(Port,Pin);
     }
     else
     {
          Pin.Pin_Speed=Output_10;
          Pin.Pin_Mode=Alternate_Output_Push_Pull;
          Pin.Pin_Number=MISO;
          MCAL_GPIO_Initialize(Port,Pin);
     }
     /*MOSI Pin Only Full duplex*/
     if(SPI_Config->Mode==SPI_Master)
     {
          Pin.Pin_Speed=Output_10;
          Pin.Pin_Mode=Alternate_Output_Push_Pull;
          Pin.Pin_Number=MOSI;
          MCAL_GPIO_Initialize(Port,Pin);
     }
     else
     {
          Pin.Pin_Speed=Input;
          Pin.Pin_Mode=Input_Floating;
          Pin.Pin_Number=MOSI;
          MCAL_GPIO_Initialize(Port,Pin);
     }
     /*Enable SPI*/
     Local_CR1.Bits.Bit_6=Enable;
     /*SPI Set Mode*/
     Local_CR1.Bits.Bit_2=SPI_Config->Mode;
     /*Communication Mode*/
     Local_CR1.Bits.Bit_10=Get_Bit(SPI_Config->Communication,ZERO);
     Local_CR1.Bits.Bit_14=Get_Bit(SPI_Config->Communication,ONE);
     Local_CR1.Bits.Bit_15=Get_Bit(SPI_Config->Communication,TWO);
     /*Frame Format*/
     Local_CR1.Bits.Bit_7=SPI_Config->Frame;
     /*Frame Size*/
     Local_CR1.Bits.Bit_11=SPI_Config->Size;
     /*Clock Polarity And Phase*/
     Local_CR1.Bits.Bit_0=Get_Bit(SPI_Config->Phase,ZERO);
     Local_CR1.Bits.Bit_1=Get_Bit(SPI_Config->Phase,ONE);
     /*Slave Select*/
     Local_CR2.Bits.Bit_2=Get_Bit(SPI_Config->Slave_Select,ZERO);
     Local_CR1.Bits.Bit_8=Get_Bit(SPI_Config->Slave_Select,ONE);
     Local_CR1.Bits.Bit_9=Get_Bit(SPI_Config->Slave_Select,TWO);
     /*Configure Pre-Scalar*/
     Write_Bits(Local_CR1.Register,THREE,THREE,SPI_Config->Prescalar);
     /*Configure Interrupt*/
     NVIC_Interrupt((NVIC_Interrupt_Numbers_t)SPI_Config->SPI_Number,Enable);
     switch (SPI_Config->Interrupt)
     {
          case SPI_Disable_Interrupt:Local_CR2.Bits.Bit_7=Disable;Local_CR2.Bits.Bit_6=Disable;NVIC_Interrupt((NVIC_Interrupt_Numbers_t)SPI_Config->SPI_Number,Disable);break;
          case SPI_Rx_Interrupt:Local_CR2.Bits.Bit_6=Enable;break;
          case SPI_Tx_Interrupt:Local_CR2.Bits.Bit_7=Enable;break;
     }
     SPIX->CR1=Local_CR1;
     SPIX->CR2=Local_CR2;
}
/********************************************************************
* Syntax          : static u64 SPI_Get_PLL2_Clock(void)
* Description     : Get PLL2 Clock
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : Void
* Parameters (out): None
* Return value:   : u64 Frequency in HZ
********************************************************************/
SPI_IRQHandler(1)
{
     SPI_IRQ_Call_Back_Function[0]();
}
SPI_IRQHandler(2)
{
     SPI_IRQ_Call_Back_Function[1]();
}
SPI_IRQHandler(3)
{
     SPI_IRQ_Call_Back_Function[2]();
}
/********************************************************************
 *  END OF FILE:  SPI_Program.c
********************************************************************/
