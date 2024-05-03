/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  USART_Interface.h
 *  Module:  MCAL/USART
 *  Description:  Universal Synchronous Asynchronous Receive Transmit Driver Interface File
*******************************************************************/
#ifndef _USART_INTERFACE_H_
#define _USART_INTERFACE_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "STM32F103.h"
#include "RCC_Interface.h"
#include "NVIC_Interface.h"
#include "GPIO_Interface.h"
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
/*------------- USART Mode -------------*/
typedef enum USART_Mode_t
{
     USART_Rx                 =(1),
     USART_Tx                 =(2),
     USART_Rx_Tx              =(3)
}USART_Mode_t;
/*------------- Bits Lenght ------------*/
typedef enum USART_Length_t
{
     USART_Eight_Bits         =(0),
     USART_Nine_Bits          =(1)
}USART_Length_t;
/*------------- Baud Rate -------------*/
typedef enum USART_Baudrate_t
{
     USART_2400               =(2400),
     USART_9600               =(9600),
     USART_19200              =(19200),
     USART_57600              =(57600),
     USART_115200             =(115200)
}USART_Baudrate_t;
/*------------ Stop Bits --------------*/
typedef enum USART_Stop_t
{
     USART_One_Stop           =(0),
     USART_Half_Stop          =(1),
     USART_Two_Stop           =(2),
     USART_One_Half_Stop      =(3),
}USART_Stop_t;
/*----------- USART Number ------------*/
typedef enum USART_Number_t
{
     USART_1                  =(37),
     USART_2                  =(38),
     USART_3                  =(39)
}USART_Number_t;
/*----------- USART Parity ------------*/
typedef enum USART_Parity_t
{
     USART_No_Parity          =(0),
     USART_Even_Parity        =(2),
     USART_Odd_Parity         =(3)
}USART_Parity_t;
/*--------- USART Interrupts ----------*/
typedef enum USART_Interrupts_t
{
     USART_Disable_Interrupt          =(0),
     USART_Parity_Error_Interrupt     =(8),
     USART_Tx_Complete_Interrupt      =(6),
     USART_Rx_Complete_Interrupt      =(5)
}USART_Interrupts_t;
/*--------- USART Configure -----------*/
typedef struct __attribute__((packed)) USART_Config_t
{
     void (*Call_Back_Function)(u16* Data);
     USART_Number_t USART_Number;
     USART_Parity_t USART_Parity;
     USART_Mode_t USART_Mode;
     logic_t USART_Flow_Control;
     USART_Length_t USART_Length;
     USART_Stop_t USART_Stop;
     USART_Baudrate_t USART_Baudrate;
     USART_Interrupts_t USART_Interrupt;
}USART_Config_t;
/*****************************************
---------    Configure Macros    ---------
*****************************************/
#define USART_IRQHandler(...)            extern void USART##__VA_ARGS__##_IRQHandler(void);extern void USART##__VA_ARGS__##_IRQHandler(void)
#define UART_Interrupt_Example           {NULL,USART_1,USART_No_Parity,USART_Rx_Tx,Disable,USART_Eight_Bits,USART_One_Stop,USART_2400,USART_Rx_Complete_Interrupt}
#define UART_Example                     {NULL,USART_1,USART_No_Parity,USART_Rx_Tx,Disable,USART_Eight_Bits,USART_One_Stop,USART_2400,USART_Disable_Interrupt}
/*****************************************
---  Application Programming Interface  --
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
void USART_Send_Array(USART_Config_t *USART_Config,u8 Array[],u8 Size);
void USART_Send_String(USART_Config_t *USART_Config,const u8 String[]);
void USART_Transmit(USART_Config_t *USART_Config,u16 Data);
void USART_Initialization(USART_Config_t *USART_Config);
void USART_Reset(USART_Config_t *USART_Config);
u16 USART_Receive(USART_Config_t *USART_Config);
/********************************************************************
 *  END OF FILE:  USART_Interface.h
********************************************************************/
#endif
