/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  EXTI_Interface.h
 *  Module:  MCAL/EXTI
 *  Description:  External Interrupt Device Driver Interface File
*******************************************************************/
#ifndef _EXTI_INTERFACE_H_
#define _EXTI_INTERFACE_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "GPIO_Interface.h"
#include "RCC_Interface.h"
#include "NVIC_Interface.h"
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
typedef enum EXTI_Sense_t
{
     Faling_Edge         =1,
     Rising_Edge         =2,
     Faling_Rising       =3
}EXTI_Sense_t;
/*----------- EXTI Config -------------*/
typedef struct __attribute__((packed)) EXTI_Config_t
{
     GPIO_Port_t Port;
     GPIO_Pin_Number_t Pin;
     EXTI_Sense_t Sensing_Edge;
     logic_t Interrupt;
     logic_t Event;
     void(*volatile Ptr_Function)(void);
}EXTI_Config_t;
/*****************************************
---------    Configure Macros    ---------
*****************************************/
#define EXTI_Pins_Victor_Map             {NVIC_EXTI0,NVIC_EXTI1,NVIC_EXTI2,NVIC_EXTI3,NVIC_EXTI4,NVIC_EXTI5,NVIC_EXTI6,NVIC_EXTI7,NVIC_EXTI8,\
                                         NVIC_EXTI9,NVIC_EXTI10,NVIC_EXTI11,NVIC_EXTI12,NVIC_EXTI13,NVIC_EXTI14,NVIC_EXTI15}
#define EXTI_IRQHandler(...)            extern void EXTI##__VA_ARGS__##_IRQHandler(void);extern void EXTI##__VA_ARGS__##_IRQHandler(void)
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
void EXTI_Initilization(EXTI_Config_t Configure);
/********************************************************************
 *  END OF FILE:  EXTI_Interface.h
********************************************************************/
#endif
