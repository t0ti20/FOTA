/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  RCC_Interface.h
 *  Module:  MCAL/RCC
 *  Description:  Reset And Clock Control Driver Interface File
*******************************************************************/
#ifndef _RCC_INTERFACE_H_
#define _RCC_INTERFACE_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "STM32F103.h"
/*****************************************
---------    Configure Macros    ---------
*****************************************/
#ifndef HSE_OSC
#define HSE_OSC                         (12000000UL)
#endif
/*------------- RCC_AHBENR -------------*//*+0*/
#define AHBENR_Base                     (0)
#define DMA1EN                          (0)       /*DMA1 clock enable*/
#define DMA2EN                          (1)       /*DMA2 clock enable*/
#define SRAMEN                          (2)       /*SRAM interface clock enable*/
#define FLITFEN                         (4)       /*FLITF clock enable*/
#define CRCEN                           (6)       /*CRC clock enable*/
/*------------- RCC_APB2ENR ------------*//*+32*/
#define APB2ENR_Base                    (1)
#define SPI1EN                          (44)      /*SPI1 Clock Enable*/
#define AFIOEN                          (32)      /*Alternate function IO clock enable*/
#define IOPAEN                          (34)      /*IO port A clock enable*/
#define IOPBEN                          (35)      /*IO port B clock enable*/
#define IOPCEN                          (36)      /*IO port C clock enable*/
#define IOPDEN                          (37)      /*IO port D clock enable*/
#define IOPEEN                          (38)      /*IO port E clock enable*/
#define USART1EN                        (46)      /*USART1 clock enable*/
/*------------- RCC_APB1ENR ------------*//*+64*/
#define APB1ENR_Base                    (2)
#define SPI2EN                          (78)      /*SPI2 Clock Enable*/
#define I2C1EN                          (85)      /*I2C1 Clock Enable*/
#define I2C2EN                          (86)      /*I2C2 Clock Enable*/
#define SPI3EN                          (79)      /*SPI3 Clock Enable*/
#define USART2EN                        (81)      /*USART2 clock enable*/
#define USART3EN                        (82)      /*USART3 clock enable*/
/*----------- HSI Internal ------------*/
#define HSI_RC                          (8000000UL)
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
/*----------- Module Number ------------*/
typedef enum RCC_Modules_t
{
     /*RCC_AHBENR*/
     RCC_DMA1            =(DMA1EN),
     RCC_DMA2            =(DMA2EN),
     RCC_SRAM            =(SRAMEN),
     RCC_FLITF           =(FLITFEN),
     RCC_CRC             =(CRCEN),
     /*RCC_APB2ENR*/
     RCC_SPI1            =(SPI1EN),
     RCC_GPIOA           =(IOPAEN),
     RCC_GPIOB           =(IOPBEN),
     RCC_GPIOC           =(IOPCEN),
     RCC_GPIOD           =(IOPDEN),
     RCC_GPIOE           =(IOPEEN),
     RCC_USART1          =(USART1EN),
     RCC_Alternative     =(AFIOEN),
     /*RCC_APB1ENR*/
     RCC_I2C1            =(I2C1EN),
     RCC_I2C2            =(I2C2EN),
     RCC_SPI2            =(SPI2EN),
     RCC_SPI3            =(SPI3EN),
     RCC_USART2          =(USART2EN),
     RCC_USART3          =(USART3EN)
}RCC_Modules_t;
/*----------- Module Number ------------*/
typedef enum RCC_Clock_t
{
     /*RCC_System_Clock*/  
     RCC_HSI              =(0),
     RCC_HSE              =(1),
     RCC_PLL              =(2),
     /*RCC_APB2ENR*//*RCC_AHBENR*//*RCC_APB1ENR*/
     RCC_PLL1_X4          =(2),
     RCC_PLL1_X5          =(3),
     RCC_PLL1_X6          =(4),
     RCC_PLL1_X7          =(5),
     RCC_PLL1_X8          =(6),
     RCC_PLL1_X9          =(7),
     RCC_PLL1_X6_5        =(13),
     RCC_PLL2_X9          =(7),
     RCC_PLL2_X10         =(8),
     RCC_PLL2_X11         =(9),
     RCC_PLL2_X12         =(10),
     RCC_PLL2_X13         =(11),
     RCC_PLL2_X14         =(12),
     RCC_PLL2_X16         =(14),
     RCC_PLL2_X20         =(15)
}RCC_Clock_t;
/*****************************************
---  Application Programming Interface  --
*****************************************/
/********************************************************************
* Syntax          : void RCC_Clock(RCC_Modules_t RCC_Module,logic_t State)
* Description     : Enable And Disable Clock For Specific Module
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (RCC_Modules_t Module Name),(logic_t State)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void RCC_Clock(RCC_Modules_t RCC_Module,logic_t State);
u64 RCC_Get_System_Clock(void);
u64 RCC_Get_AHB_Clock(void);
u64 RCC_Get_APB1_Clock(void);
u64 RCC_Get_APB2_Clock(void);
/********************************************************************
 *  END OF FILE:  RCC_Interface.h
********************************************************************/
#endif
