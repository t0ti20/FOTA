/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  NVIC_Interface.h
 *  Module:  MCAL/NVIC
 *  Description:  Nested Victor Interrupt Controller Driver Interface File
*******************************************************************/
#ifndef _NVIC_INTERFACE_H_
#define _NVIC_INTERFACE_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "STM32F103.h"
#include "RCC_Interface.h"
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
typedef enum NVIC_Interrupt_Numbers_t
{
     NVIC_Hard_Fault      =(0),
     NVIC_Memory_Manage   =(1),           
     NVIC_Bus_Fault       =(2),
     NVIC_Usage_Fault     =(3),
     NVIC_SV_Call         =(4),
     NVIC_Debug_Monitor   =(5),
     NVIC_Pend_SV         =(6),
     NVIC_Sys_Tick        =(7),
     NVIC_WWDG            =(0),
     NVIC_PVD             =(1),
     NVIC_TAMPER          =(2),
     NVIC_RTC             =(3),
     NVIC_FLASH           =(4),
     NVIC_RCC             =(5),
     NVIC_EXTI0           =(6),
     NVIC_EXTI1           =(7),
     NVIC_EXTI2           =(8),
     NVIC_EXTI3           =(9),
     NVIC_EXTI4           =(10),
     NVIC_EXTI5           =(23),
     NVIC_EXTI6           =(23),
     NVIC_EXTI7           =(23),
     NVIC_EXTI8           =(23),
     NVIC_EXTI9           =(23),
     NVIC_SPI1            =(35),
     NVIC_SPI2            =(36),
     NVIC_I2C1_EV         =(31),
     NVIC_I2C1_ER         =(32),
     NVIC_I2C2_EV         =(33),
     NVIC_I2C2_ER         =(34),
     NVIC_USART1          =(37),
     NVIC_USART2          =(38),
     NVIC_USART3          =(39),
     NVIC_EXTI10          =(40),
     NVIC_EXTI11          =(40),
     NVIC_EXTI12          =(40),
     NVIC_EXTI13          =(40),
     NVIC_EXTI14          =(40),
     NVIC_EXTI15          =(40),
     NVIC_SPI3            =(51)

}NVIC_Interrupt_Numbers_t;
/*****************************************
---------    Configure Macros    ---------
*****************************************/

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
void NVIC_Interrupt(NVIC_Interrupt_Numbers_t Interrupt_Number,logic_t State);
/********************************************************************
 *  END OF FILE:  NVIC_Interface.h
********************************************************************/
#endif
