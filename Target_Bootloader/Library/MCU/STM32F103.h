/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  STM32F103.h
 *  Module:  Library/MCU
 *  Description:  Headder File For MCU "STM32F103x6"
*******************************************************************/
#ifndef _STM32F103_H_
#define _STM32F103_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "Standard_Types.h"
#include "Macros.h"
/*****************************************
------    Memory Base Addresses    -------
*****************************************/
#define ROM_BASE                        (0x08000000UL)
#define RAM_BASE                        (0x20000000UL)
#define System_BASE                     (0x1FFFF000UL)
#define Pheriphals_BASE                 (0x40000000UL)
#define Pheriphals_Alias_BASE           (0x42000000UL)
#define Internal_Pheriphals_BASE        (0xE0000000UL)
/*****************************************
---------  Peripheral BitBand  -----------
*****************************************/
/*--------------------------------------*/
/* 0x40000000 >>>>>>>>>>>>>> 0x400FFFFF */
/*--------------------------------------*/
#define Peripheral_BitBand(Register,Bit)     (*((volatile u32*)(Pheriphals_Alias_BASE+(((((u32)&(Register)))-Pheriphals_BASE)*(32))+(Bit*(4)))))
/*****************************************
------   Pheriphal Base Addresses  -------
*****************************************/
#define SYSTICK_Offset                  (0xE010UL)
#define SCB_Offset                      (0xED00UL)
/*--------------------------------------*/
/* 0x40000000 >>>>>>>>>>>>>> 0x4000FFFF */
/*--------------- APB1 -----------------*/
#define TIM2_Offset                     (0x00000UL)
#define SPI2_Offset                     (0x03800UL)
#define SPI3_Offset                     (0x03C00UL)
#define I2C1_Offset                     (0x05400UL)
#define I2C2_Offset                     (0x05800UL)
/*--------------------------------------*/
/* 0x40010000 >>>>>>>>>>>>>> 0x40017FFF */
/*--------------- APB2 -----------------*/
#define SPI1_Offset                     (0x13000UL)
#define USART3_Offset                   (0x04800UL)
#define USART2_Offset                   (0x04400UL)
#define USART1_Offset                   (0x13800UL)
#define NVIC_Offset                     (0x0E100UL)
#define AFIO_Offset                     (0x10000UL)
#define EXTI_Offset                     (0x10400UL)
#define GBIOA_Offset                    (0x10800UL)
#define GBIOB_Offset                    (0x10C00UL)
#define GBIOC_Offset                    (0x11000UL)
#define GBIOD_Offset                    (0x11400UL)
#define GBIOE_Offset                    (0x11800UL)
/*--------------------------------------*/
/* 0x40018000 >>>>>>>>>>>>>> 0xA0000FFF */
/*---------------- AHB -----------------*/
#define RCC_Offset                      (0x21000UL)
#define CRC_Offset                      (0x23000UL)
#define FLASH_Offset                    (0x22000UL)
/****************************************
 *          Generic Macros              *
*****************************************/
/*------------- BitField -------------*/
typedef struct Bits_t
{
     volatile u32 Bit_0:ONE;volatile u32 Bit_1:ONE;volatile u32 Bit_2:ONE;
     volatile u32 Bit_3:ONE;volatile u32 Bit_4:ONE;volatile u32 Bit_5:ONE;
     volatile u32 Bit_6:ONE;volatile u32 Bit_7:ONE;volatile u32 Bit_8:ONE;
     volatile u32 Bit_9:ONE;volatile u32 Bit_10:ONE;volatile u32 Bit_11:ONE;
     volatile u32 Bit_12:ONE;volatile u32 Bit_13:ONE;volatile u32 Bit_14:ONE;
     volatile u32 Bit_15:ONE;volatile u32 Bit_16:ONE;volatile u32 Bit_17:ONE;
     volatile u32 Bit_18:ONE;volatile u32 Bit_19:ONE;volatile u32 Bit_20:ONE;
     volatile u32 Bit_21:ONE;volatile u32 Bit_22:ONE;volatile u32 Bit_23:ONE;
     volatile u32 Bit_24:ONE;volatile u32 Bit_25:ONE;volatile u32 Bit_26:ONE;
     volatile u32 Bit_27:ONE;volatile u32 Bit_28:ONE;volatile u32 Bit_29:ONE;
     volatile u32 Bit_30:ONE;volatile u32 Bit_31:ONE;
}Bits_t;
typedef union Register_t
{
     volatile u32 Register;
     Bits_t Bits;
}Register_t;
/****************************************
 *        Pheripheral Registers         *
*****************************************/
/*---------------- USART ---------------*/
typedef struct SPI_t
{
     Register_t CR1;                 /*0x00*/
     Register_t CR2;                 /*0x04*/
     Register_t SR;                  /*0x08*/
     Register_t DR;                  /*0x0C*/
     Register_t CRCPR;               /*0x10*/
     Register_t RXCRCR;              /*0x14*/
     Register_t TXCRCR;              /*0x18*/
     Register_t I2SCFGR;             /*0x1C*/
     Register_t I2SPR;               /*0x20*/
}SPI_t;
/*---------------- USART ---------------*/
typedef struct USART_t
{
     Register_t SR;                   /*0x00*/
     Register_t DR;                   /*0x04*/
     Register_t BRR;                  /*0x08*/
     Register_t CR1;                  /*0x0C*/
     Register_t CR2;                  /*0x10*/
     Register_t CR3;                  /*0x14*/
     Register_t GTPR;                 /*0x18*/
}USART_t;
/*---------------- EXTI ----------------*/
typedef struct EXTI_t
{
     Register_t IMR;                  /*0x00*/
     Register_t EMR;                  /*0x04*/
     Register_t RTSR;                 /*0x08*/
     Register_t FTSR;                 /*0x0C*/
     Register_t SWIER;                /*0x10*/
     Register_t PR;                   /*0x14*/    
}EXTI_t;
/*---------------- GPIO ----------------*/
typedef struct GPIOX_t
{
     Register_t CRL;                  /*0x00*/
     Register_t CRH;                  /*0x04*/
     Register_t IDR;                  /*0x08*/
     Register_t ODR;                  /*0x0C*/
     Register_t BSRR;                 /*0x10*/
     Register_t BRR;                  /*0x14*/  
     Register_t LCKR;                 /*0x18*/
}GPIOX_t;
/*---------------- AFIO ----------------*/
typedef struct AFIO_t
{
     Register_t EVCR;                 /*0x00*/
     Register_t MAPR;                 /*0x04*/
     Register_t EXTICRX[4];           /*0x08->0x14*/
     Register_t Reserved;             /*0x18*/
     Register_t AFIO_MAPR2;           /*0x1C*/
}AFIO_t;
/*---------------- I2C ----------------*/
typedef struct I2C_t
{
     Register_t CR1;                  /*0x00*/
     Register_t CR2;                  /*0x04*/
     Register_t OAR1;                 /*0x08*/
     Register_t OAR2;                 /*0x0C*/
     Register_t DR;                   /*0x10*/
     Register_t SR1;                  /*0x14*/
     Register_t SR2;                  /*0x18*/
     Register_t CCR;                  /*0x1C*/
     Register_t TRISE;                /*0x20*/
}I2C_t;
/*---------------- RCC ----------------*/
typedef struct RCC_t
{
     Register_t CR;                     /*0x00*/
     Register_t CFGR;                   /*0x04*/
     Register_t CIR;                    /*0x08*/
     Register_t APB2RSTR;               /*0x0C*/
     Register_t APB1RSTR;               /*0x10*/
     Register_t AHBENR;                 /*0x14*/
     Register_t APB2ENR;                /*0x18*/
     Register_t APB1ENR;                /*0x1C*/
     Register_t BDCR;                   /*0x20*/
     Register_t CSR;                    /*0x24*/
     Register_t AHBSTR;                 /*0x28*/
     Register_t CFGR2;                  /*0x2C*/
}RCC_t;
/*--------------- NVIC ----------------*/
typedef struct NVIC_t
{
     Register_t ISERX[3];               /*0x000->0x00C*/ 
     Register_t Reserved_1[29];
     Register_t ICERX[3];               /*0x080->0x08C*/ 
     Register_t Reserved_2[29];
     Register_t ISPRX[3];               /*0x100->0x10C*/     
     Register_t Reserved_3[29];
     Register_t ICPRX[3];               /*0x180->0x18C*/ 
     Register_t Reserved_4[29];
     Register_t IABRX[3];               /*0x200->0x20C*/ 
     Register_t Reserved_5[61];
     Register_t IPRX[21];               /*0x300->0x304*/ 
     Register_t Reserved_6[1591];
     Register_t STIR;                   /*0xE00->0xE04*/ 
}NVIC_t;
/*---------------- SCB ----------------*/
typedef struct SCB_t
{
    Register_t CPUID;                   /*0x00*/
    Register_t ICSR;                    /*0x04*/
    Register_t VTOR;                    /*0x08*/
    Register_t AIRCR;                   /*0x0C*/
    Register_t SCR;                     /*0x10*/
    Register_t CCR;                     /*0x14*/
    Register_t SHPR[3];                 /*0x18 -> 0x20*/
    Register_t SHCSR;                   /*0x24*/
    Register_t CFSR;                    /*0x28*/
    Register_t HFSR;                    /*0x2C*/
    Register_t DFSR;                    /*0x30*/
    Register_t MMFAR;                   /*0x34*/
    Register_t BFAR;                    /*0x35*/
    Register_t AFSR;                    /*0x3C*/
}SCB_t;
/*-------------- SysTick --------------*/
typedef struct SysTick_t
{
    Register_t CTRL;                    /*0x00*/
    Register_t LOAD;                    /*0x04*/
    Register_t VAL;                     /*0x08*/
    const Register_t CALIB;             /*0x0C*/
}SysTick_t;
/*---------------- CRC ----------------*/
typedef struct CRC_t
{
     Register_t DR;                     /*0x00*/
     Register_t IDR;                    /*0x04*/
     Register_t CR;                     /*0x08*/
}CRC_t;
/*--------------- FLASH ---------------*/
typedef struct FLASH_t
{
     Register_t ACR;                    /*0x00*/
     Register_t KEYR;                   /*0x04*/
     Register_t OPTKEYR;                /*0x08*/
     Register_t SR;                     /*0x0C*/
     Register_t CR;                     /*0x10*/
     Register_t AR;                     /*0x14*/
     Register_t Reserved;               /*0x18*/
     Register_t OBR;                    /*0x1C*/
     Register_t WRPR;                   /*0x20*/
}FLASH_t;
/*-------------- DBGMCU ---------------*/
#define UNIQUE_DEVICE_ID_REGISTER       (*((volatile u32*)(0x1FFFF7E8)))
/****************************************
 *         Pheripheral Instants         *
*****************************************/
#define I2C1                            (*((I2C_t*)(Pheriphals_BASE+I2C1_Offset)))
#define I2C2                            (*((I2C_t*)(Pheriphals_BASE+I2C2_Offset)))
#define SPI1                            (*((SPI_t*)(Pheriphals_BASE+SPI1_Offset)))
#define SPI2                            (*((SPI_t*)(Pheriphals_BASE+SPI2_Offset)))
#define SPI3                            (*((SPI_t*)(Pheriphals_BASE+SPI3_Offset)))
#define RCC                             (*((RCC_t*)(Pheriphals_BASE+RCC_Offset)))
#define CRC                             (*((CRC_t*)(Pheriphals_BASE+CRC_Offset)))
#define AFIO                            (*((AFIO_t*)(Pheriphals_BASE+AFIO_Offset)))
#define EXTI                            (*((EXTI_t*)(Pheriphals_BASE+EXTI_Offset)))
#define FLASH                           (*((FLASH_t*)(Pheriphals_BASE+FLASH_Offset)))
#define GPIOA                           (*((GPIOX_t*)(Pheriphals_BASE+GBIOA_Offset)))
#define GPIOB                           (*((GPIOX_t*)(Pheriphals_BASE+GBIOB_Offset)))
#define GPIOC                           (*((GPIOX_t*)(Pheriphals_BASE+GBIOC_Offset)))
#define GPIOD                           (*((GPIOX_t*)(Pheriphals_BASE+GBIOD_Offset)))
#define GPIOE                           (*((GPIOX_t*)(Pheriphals_BASE+GBIOE_Offset)))
#define USART1                          (*((USART_t*)(Pheriphals_BASE+USART1_Offset)))
#define USART2                          (*((USART_t*)(Pheriphals_BASE+USART2_Offset)))
#define USART3                          (*((USART_t*)(Pheriphals_BASE+USART3_Offset)))
#define SCB                             (*((SCB_t*)(Internal_Pheriphals_BASE+SCB_Offset)))
#define NVIC                            (*((NVIC_t*)(Internal_Pheriphals_BASE+NVIC_Offset)))
#define SysTick                         (*((SysTick_t*)(Internal_Pheriphals_BASE+SYSTICK_Offset)))
/********************************************************************
 *  END OF FILE: STM32F103.h
********************************************************************/
#endif
