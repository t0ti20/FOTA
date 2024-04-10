/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  SPI_Interface.h
 *  Module:  MCAL/SPI
 *  Description:  Reset And Clock Control Driver Interface File
*******************************************************************/
#ifndef _SPI_INTERFACE_H_
#define _SPI_INTERFACE_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "STM32F103.h"
#include "RCC_Interface.h"
#include "NVIC_Interface.h"
#include "GPIO_Interface.h"
/*****************************************
---------    Configure Macros    ---------
*****************************************/
/*----------- HSI Internal ------------*/
#define HSI_RC (8000000UL)
/*----------- IRQ Generator -----------*/
#define SPI_IRQHandler(...)            extern void SPI##__VA_ARGS__##_IRQHandler(void);extern void SPI##__VA_ARGS__##_IRQHandler(void)
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
/*-------------- SPI Mode --------------*/
/*SSM.9//SSI.8//2//SSOE.2*/
typedef enum SPI_SS_t
{
     SPI_Hardware_Slave = (0),
     SPI_Hardware_Master_Tx = (1),
     SPI_Hardware_Master_Rx = (0),
     SPI_Software_Enable = (6),
     SPI_Software_Disable = (4)
} SPI_SS_t;
/*-------------- SPI Mode --------------*/
typedef enum SPI_Interrupt_t
{
     SPI_Disable_Interrupt = (0),
     SPI_Rx_Interrupt = (1),
     SPI_Tx_Interrupt = (2)
} SPI_Interrupt_t;
/*-------------- SPI Mode --------------*/
typedef enum SPI_Prescalar_t
{
     SPI_Prescalar_2 = (0),
     SPI_Prescalar_4 = (1),
     SPI_Prescalar_8 = (2),
     SPI_Prescalar_16 = (3),
     SPI_Prescalar_32 = (4),
     SPI_Prescalar_64 = (5),
     SPI_Prescalar_128 = (6),
     SPI_Prescalar_256 = (7)
} SPI_Prescalar_t;
/*-------------- SPI Mode --------------*/
typedef enum SPI_Phase_t
{
     SPI_Idle_Low_First_Edge = (0),
     SPI_Idle_Low_Second_Edge = (1),
     SPI_Idle_Hi_First_Edge = (2),
     SPI_Idle_Hi_Second_Edge = (3)
} SPI_Phase_t;
/*-------------- SPI Mode --------------*/
typedef enum SPI_Size_t
{
     SPI_8_Bit = (0),
     SPI_16_Bit = (1)
} SPI_Size_t;
/*-------------- SPI Mode --------------*/
typedef enum SPI_Frame_t
{
     SPI_MSBF = (0),
     SPI_LSBF = (1)
} SPI_Frame_t;
/*---------- SPI Communication ---------*/
typedef enum SPI_Communication_t
{    //0b/15/14/10
     SPI_Tx_Rx_Two_Line = (0),
     SPI_Rx_Two_Line = (1),
     SPI_Rx_One_Line = (4),
     SPI_Tx_One_Line = (6)
} SPI_Communication_t;
/*-------------- SPI Mode --------------*/
typedef enum SPI_Mode_t
{
     SPI_Slave = (0),
     SPI_Master = (1)
} SPI_Mode_t;
/*-------------- SPI Mode --------------*/
typedef enum SPI_Number_t
{
     SPI_1 = (35),
     SPI_2 = (36),
     SPI_3 = (51)
} SPI_Number_t;
/*----------- SPI Configure ------------*/
typedef struct __attribute__((packed)) SPI_Config_t
{
     void(*Call_Back_Function)(void);
     SPI_Number_t SPI_Number;
     SPI_Mode_t Mode;
     SPI_Communication_t Communication;
     SPI_Frame_t Frame;
     SPI_Size_t Size;
     SPI_Phase_t Phase;
     SPI_Prescalar_t Prescalar;
     SPI_Interrupt_t Interrupt;
     SPI_SS_t Slave_Select;
} SPI_Config_t;
/*****************************************
---  Application Programming Interface  --
*****************************************/
/********************************************************************
 * Syntax          : void SPI_Clock(SPI_Modules_t SPI_Module,logic_t State)
 * Description     : Enable And Disable Clock For Specific Module
 * Sync-Async      : *
 * Reentrancy      : *
 * Parameters (in) : (SPI_Modules_t Module Name),(logic_t State)
 * Parameters (out): None
 * Return value:   : Void
 ********************************************************************/
void SPI_Initialization(SPI_Config_t *SPI_Config);
void SPI_Receive_Data(SPI_Config_t *SPI_Config,u16 *Data);
void SPI_Send_Data(SPI_Config_t *SPI_Config,u16 Data);
void SPI_Send_Receive_Data(SPI_Config_t *SPI_Config,u16 *Data);
/********************************************************************
 *  END OF FILE:  SPI_Interface.h
 ********************************************************************/
#endif
