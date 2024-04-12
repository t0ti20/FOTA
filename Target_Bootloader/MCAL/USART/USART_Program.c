/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  USART_Program.c
 *  Module:  MCAL/USART
 *  Description:  Universal Synchronous Asynchronous Receive Transmit Driver Program File For STM32F103C6
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "USART_Interface.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
static void (*USART_IRQ_Call_Back_Function[3])(u16 *Data)={NULL};
/********************************************************************
* Syntax          : void USART_Initialization(USART_Config_t *USART_Config)
* Description     : Initialize USART Regesters
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To USART_Config_t)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void USART_Send_Array(USART_Config_t *USART_Config,u8 Array[],u8 Size)
{
     while(Size)
     {
          USART_Transmit(USART_Config,Array[--Size]);
     }
}
void USART_Reset(USART_Config_t *USART_Config)
{
     USART_t *USART_Number=NULL;
     /* Select Which USART To Configure */
     switch (USART_Config->USART_Number)
     {
          case USART_1:USART_Number=&USART1;break;
          case USART_2:USART_Number=&USART2;break;
          case USART_3:USART_Number=&USART3;break;
     }
    /* Disable USART1 */
    USART_Number->CR1.Bits.Bit_13=Disable;
    /* Reset USART1 registers to default values */
    USART_Number->CR1.Register=ZERO;
    USART_Number->CR2.Register=ZERO;
    USART_Number->CR3.Register=ZERO;
    USART_Number->BRR.Register=ZERO;
    /* Clear USART1 status registers */
    USART_Number->SR.Register = ZERO;
    /* Set UE bit */
    //USART_Number->CR1.Bits.Bit_13=ENABLE;
}
void USART_Send_String(USART_Config_t *USART_Config,const u8 String[])
{
     do
     {
          USART_Transmit(USART_Config,String[0]);
          String++;
     }while(String[0]);
     USART_Transmit(USART_Config,String[0]);
}
void USART_Initialization(USART_Config_t *USART_Config)
{
     GPIO_Port_t Port_Configure={GPIO_A};
     GPIO_Pin_Config_t Pin_Configure={Pin_9,Alternate_Output_Push_Pull,Output_10};
     GPIO_Pin_Number_t Tx=Pin_9,Rx=Pin_10,CTS=Pin_11,RTS=Pin_12;
     USART_t *USART_Number=NULL;
     u64 Module_Clock=ZERO;
     /*Select Which USART To Configure*/
     switch (USART_Config->USART_Number)
     {
          case USART_1:
               Port_Configure=GPIO_A;Tx=Pin_9;Rx=Pin_10;CTS=Pin_11;RTS=Pin_12;
               Module_Clock=RCC_Get_APB2_Clock();
               RCC_Clock(RCC_USART1,ENABLE);
               USART_Number=&USART1;
          break;
          case USART_2:
               Port_Configure=GPIO_A;Tx=Pin_2;Rx=Pin_3;CTS=Pin_0;RTS=Pin_1;
               Module_Clock=RCC_Get_APB1_Clock();
               RCC_Clock(RCC_USART2,ENABLE);
               USART_Number=&USART2;
          break;
          case USART_3:
               Port_Configure=GPIO_B;Tx=Pin_10;Rx=Pin_11;CTS=Pin_13;RTS=Pin_14;
               Module_Clock=RCC_Get_APB1_Clock();
               RCC_Clock(RCC_USART3,ENABLE);
               USART_Number=&USART3;
          break;
     }
     /*Set Tx Pins For USART*/
     Pin_Configure.Pin_Number=Tx;
     MCAL_GPIO_Initialize(Port_Configure,Pin_Configure);
     /*Set CTS And RTC Pins For USART*/
     if(USART_Config->USART_Flow_Control)
     {
          Pin_Configure.Pin_Number=RTS;
          MCAL_GPIO_Initialize(Port_Configure,Pin_Configure);
          Pin_Configure.Pin_Speed=Input;
          Pin_Configure.Pin_Mode=Alternate_Input;
          Pin_Configure.Pin_Number=CTS;
          MCAL_GPIO_Initialize(Port_Configure,Pin_Configure);
     }
     /*Set Rx Pins For USART*/
     Pin_Configure.Pin_Number=Rx;
     Pin_Configure.Pin_Speed=Input;
     Pin_Configure.Pin_Mode=Alternate_Input;
     MCAL_GPIO_Initialize(Port_Configure,Pin_Configure);
     /*Enable the USART by writing the UE bit in USART_CR1 register to 1*/
	USART_Number->CR1.Bits.Bit_13=ENABLE;
     /*Program the M bit in USART_CR1 to define the word length*/
     USART_Number->CR1.Bits.Bit_12=USART_Config->USART_Length;
     /*Program the number of stop bits in USART_CR2*/
     Write_Bits(USART_Number->CR2.Register,12,TWO,USART_Config->USART_Stop);
     /*Set Mode Tx-Rx-TxRx */
     Write_Bits(USART_Number->CR1.Register,TWO,TWO,USART_Config->USART_Mode);
     /*Configure Parity*/
     Write_Bits(USART_Number->CR1.Register,NINE,TWO,USART_Config->USART_Parity);
     /*Flow Control*/
     USART_Number->CR1.Bits.Bit_8=USART_Config->USART_Flow_Control;
     USART_Number->CR1.Bits.Bit_9=USART_Config->USART_Flow_Control;
     /*Select the desired baud rate using the USART_BRR register.*/
     Module_Clock=(Module_Clock*25)/(USART_Config->USART_Baudrate*FOUR);
     Module_Clock=((Module_Clock/100)<<FOUR)|(((Module_Clock%100)*FOUR)/25);
     Write_Bits(USART_Number->BRR.Register,ZERO,16,(u16)Module_Clock);
     /*Configure Call Back Function*/
     USART_IRQ_Call_Back_Function[(USART_Config->USART_Number-USART_1)]=USART_Config->Call_Back_Function;
     /*Enable Disable Interrupt*/
     USART_Number->SR.Bits.Bit_6=Disable;
     if(USART_Config->USART_Interrupt)Set_Bit(USART_Number->CR1.Register,USART_Config->USART_Interrupt);
     NVIC_Interrupt((NVIC_Interrupt_Numbers_t)(USART_Config->USART_Number),Enable);
}
/********************************************************************
* Syntax          : void USART_Initialization(USART_Config_t *USART_Config)
* Description     : Initialize USART Regesters
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To USART_Config_t)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void USART_Transmit(USART_Config_t *USART_Config,u16 Data)
{
     USART_t *USART_Number=NULL;
     /*Select Which USART To Configure*/
     switch (USART_Config->USART_Number)
     {
          case USART_1:USART_Number=&USART1;break;
          case USART_2:USART_Number=&USART2;break;
          case USART_3:USART_Number=&USART3;break;
     }
     while(!(USART_Number->SR.Bits.Bit_7));
     /*Write Data*/
     Data&=USART_Config->USART_Length?0x1FF:0xFF;
     USART_Number->DR.Register=Data;
}
/********************************************************************
* Syntax          : void USART_Initialization(USART_Config_t *USART_Config)
* Description     : Initialize USART Regesters
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To USART_Config_t)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
u16 USART_Receive(USART_Config_t *USART_Config)
{
     USART_t *USART_Number=NULL;
     u16 Data=ZERO;
     /*Select Which USART To Configure*/
     switch (USART_Config->USART_Number)
     {
          case USART_1:USART_Number=&USART1;break;
          case USART_2:USART_Number=&USART2;break;
          case USART_3:USART_Number=&USART3;break;
     }
     /*Wait until data is received (wait for RXNE bit in SR register)*/
     while(!(USART_Number->SR.Bits.Bit_5));
     /*9 Bit Data No Parity*/
     if((USART_Config->USART_Length==USART_Nine_Bits)&&(USART_Config->USART_Parity==USART_No_Parity))
     {
          Data=USART_Number->DR.Register&0x1ff;
     }
     /*9 Bit Data With Parity*/
     else if((USART_Config->USART_Length==USART_Nine_Bits)&&!(USART_Config->USART_Parity==USART_No_Parity))
     {
          Data=(USART_Number->DR.Register)&0xff;
     }
     /*8 Bit Data No Parity*/
     else if((USART_Config->USART_Length==USART_Eight_Bits)&&(USART_Config->USART_Parity==USART_No_Parity))
     {
          Data=(USART_Number->DR.Register)&0xff;
     }
     /*8 Bit Data With Parity*/
     else if((USART_Config->USART_Length==USART_Eight_Bits)&&!(USART_Config->USART_Parity==USART_No_Parity))
     {
          Data=(USART_Number->DR.Register)&0x7f;
     }
     return Data;
}
/********************************************************************
* Syntax          : void USART_Initialization(USART_Config_t *USART_Config)
* Description     : Initialize USART Regesters
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To USART_Config_t)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
USART_IRQHandler(1)
{
     u16 Data=ZERO;
     /*Read data register not empty*/
     if(USART1.CR1.Bits.Bit_5&&USART1.SR.Bits.Bit_5)
     {
          /*9 Bit Data No Parity*/
          if((USART1.CR1.Bits.Bit_12==USART_Nine_Bits)&&(Get_Bits(USART1.CR1.Register,NINE,TWO)==USART_No_Parity))Data=USART1.DR.Register&0x1ff;
          /*9 Bit Data With Parity*/
          else if((USART1.CR1.Bits.Bit_12==USART_Nine_Bits)&&!(Get_Bits(USART1.CR1.Register,NINE,TWO)==USART_No_Parity))Data=(USART1.DR.Register)&0xff;
          /*8 Bit Data No Parity*/
          else if((USART1.CR1.Bits.Bit_12==USART_Eight_Bits)&&(Get_Bits(USART1.CR1.Register,NINE,TWO)==USART_No_Parity))Data=(USART1.DR.Register)&0xff;
          /*8 Bit Data With Parity*/
          else if((USART1.CR1.Bits.Bit_12==USART_Eight_Bits)&&!(Get_Bits(USART1.CR1.Register,NINE,TWO)==USART_No_Parity))Data=(USART1.DR.Register)&0x7f;   
          USART_IRQ_Call_Back_Function[0](&Data);
     }
     /*Transmit data register empty*/
     else if (USART1.CR1.Bits.Bit_6)
     {
          USART_IRQ_Call_Back_Function[0](&Data);
     }
}
USART_IRQHandler(2)
{
     u16 Data=ZERO;
     /*Read data register not empty*/
     if(USART2.CR1.Bits.Bit_5)
     {
          /*9 Bit Data No Parity*/
          if((USART2.CR1.Bits.Bit_12==USART_Nine_Bits)&&(Get_Bits(USART2.CR1.Register,NINE,TWO)==USART_No_Parity))Data=USART2.DR.Register&0x1ff;
          /*9 Bit Data With Parity*/
          else if((USART2.CR1.Bits.Bit_12==USART_Nine_Bits)&&!(Get_Bits(USART2.CR1.Register,NINE,TWO)==USART_No_Parity))Data=(USART2.DR.Register)&0xff;
          /*8 Bit Data No Parity*/
          else if((USART2.CR1.Bits.Bit_12==USART_Eight_Bits)&&(Get_Bits(USART2.CR1.Register,NINE,TWO)==USART_No_Parity))Data=(USART2.DR.Register)&0xff;
          /*8 Bit Data With Parity*/
          else if((USART2.CR1.Bits.Bit_12==USART_Eight_Bits)&&!(Get_Bits(USART2.CR1.Register,NINE,TWO)==USART_No_Parity))Data=(USART2.DR.Register)&0x7f;   
          USART_IRQ_Call_Back_Function[0](&Data);
     }
     /*Transmit data register empty*/
     else if (USART2.CR1.Bits.Bit_6)
     {
          USART_IRQ_Call_Back_Function[0](&Data);
     }
}
USART_IRQHandler(3)
{
     u16 Data=ZERO;
     /*Read data register not empty*/
     if(USART3.CR1.Bits.Bit_5)
     {
          /*9 Bit Data No Parity*/
          if((USART3.CR1.Bits.Bit_12==USART_Nine_Bits)&&(Get_Bits(USART3.CR1.Register,NINE,TWO)==USART_No_Parity))Data=USART3.DR.Register&0x1ff;
          /*9 Bit Data With Parity*/
          else if((USART3.CR1.Bits.Bit_12==USART_Nine_Bits)&&!(Get_Bits(USART3.CR1.Register,NINE,TWO)==USART_No_Parity))Data=(USART3.DR.Register)&0xff;
          /*8 Bit Data No Parity*/
          else if((USART3.CR1.Bits.Bit_12==USART_Eight_Bits)&&(Get_Bits(USART3.CR1.Register,NINE,TWO)==USART_No_Parity))Data=(USART3.DR.Register)&0xff;
          /*8 Bit Data With Parity*/
          else if((USART3.CR1.Bits.Bit_12==USART_Eight_Bits)&&!(Get_Bits(USART3.CR1.Register,NINE,TWO)==USART_No_Parity))Data=(USART3.DR.Register)&0x7f;   
          USART_IRQ_Call_Back_Function[0](&Data);
     }
     /*Transmit data register empty*/
     else if (USART3.CR1.Bits.Bit_6)
     {
          USART_IRQ_Call_Back_Function[0](&Data);
     }
}


/********************************************************************
 *  END OF FILE:  USART_Program.c
********************************************************************/
