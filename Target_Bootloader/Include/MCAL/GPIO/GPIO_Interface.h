/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  GPIO_Interface.h
 *  Module:  MCAL/GPIO
 *  Description:  General Input Output Driver Interface File
*******************************************************************/
#ifndef _GPIO_INTERFACE_H_
#define _GPIO_INTERFACE_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "STM32F103.h"
#include "RCC_Interface.h"
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
/*----------- Pins Numbers -------------*/
typedef enum GPIO_Pin_Number_t
{
     Pin_0                              =(u8)(0),     /*Pin 0 Is Selected*/
     Pin_1                              =(u8)(1),     /*Pin 1 Is Selected*/
     Pin_2                              =(u8)(2),     /*Pin 2 Is Selected*/
     Pin_3                              =(u8)(3),     /*Pin 3 Is Selected*/
     Pin_4                              =(u8)(4),     /*Pin 4 Is Selected*/
     Pin_5                              =(u8)(5),     /*Pin 5 Is Selected*/
     Pin_6                              =(u8)(6),     /*Pin 6 Is Selected*/
     Pin_7                              =(u8)(7),     /*Pin 7 Is Selected*/
     Pin_8                              =(u8)(8),     /*Pin 8 Is Selected*/
     Pin_9                              =(u8)(9),     /*Pin 9 Is Selected*/
     Pin_10                             =(u8)(10),    /*Pin 10 Is Selected*/
     Pin_11                             =(u8)(11),    /*Pin 11 Is Selected*/
     Pin_12                             =(u8)(12),    /*Pin 12 Is Selected*/
     Pin_13                             =(u8)(13),    /*Pin 13 Is Selected*/
     Pin_14                             =(u8)(14),    /*Pin 14 Is Selected*/
     Pin_15                             =(u8)(15)     /*Pin 15 Is Selected*/
}GPIO_Pin_Number_t;
/*-------------- Pin Mode --------------*/
typedef enum GPIO_Pin_Mode_t
{
     Analog                             =(u8)(0),      /*Analog mode*/
     Input_Floating                     =(u8)(4),      /*Floating input (reset state)*/
     Input_Pull_Up                      =(u8)(8),      /*Input with pull-up*/
     Input_Pull_Down                    =(u8)(15),     /*Input with pull-down*/
     Output_Push_Pull                   =(u8)(0),      /*General purpose output push-pull*/
     Output_Open_Drain                  =(u8)(4),      /*General purpose output Open-drain*/
     Alternate_Input                    =(u8)(4),      /*Alternate function Input */
     Alternate_Output_Push_Pull         =(u8)(8),      /*Alternate function output Push-pull*/
     Alternate_Output_Open_Drain        =(u8)(12)      /*Alternate function output Open-drain*/
}GPIO_Pin_Mode_t;
/*------------- Pin Speed  -------------*/
typedef enum GPIO_Pin_Speed_t
{
     Input                              =(u8)(0),      /*Input mode (reset state)*/
     Output_2                           =(u8)(2),      /*Output mode, max speed 2 MHz*/
     Output_10                          =(u8)(1),      /*Output mode, max speed 10 MHz*/
     Output_50                          =(u8)(3)       /*Output mode, max speed 50 MHz*/
}GPIO_Pin_Speed_t;
/*----------- Pin Configure  -----------*/
typedef struct GPIO_Pin_Config_t
{
     GPIO_Pin_Number_t Pin_Number;
     GPIO_Pin_Mode_t Pin_Mode;
     GPIO_Pin_Speed_t Pin_Speed;
}GPIO_Pin_Config_t;
/*----------- Ports  -----------*/
typedef enum GPIO_Port_t
{
     GPIO_A,GPIO_B,GPIO_C,GPIO_D
}
GPIO_Port_t;
/*****************************************
---------    Configure Macros    ---------
*****************************************/
#define HI                              (1)
#define LOW                             (0)
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
void MCAL_GPIO_Lock_Pin(GPIO_Port_t Port_Number,GPIO_Pin_Number_t Pin);
/********************************************************************
* Syntax          : MCAL_GPIO_Initialize(GPIOX_t *Port,GPIO_Pin_t Pin)
* Description     : Write Logic On Specific Pin
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(Pin Configuration "GPIO_Pin_t")
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void MCAL_GPIO_Initialize(GPIO_Port_t Port_Number,GPIO_Pin_Config_t Pin);

/********************************************************************
* Syntax          : MCAL_GPIO_Read_Pin(GPIOX_t *Port,GPIO_Pin_t Pin)
* Description     : Read Logic State For Specific Pin
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(Copy To Pin Number "GPIO_Pin_Number_t")
* Parameters (out): None
* Return value:   : u8 Refers To Pin State "HI-LOW"
********************************************************************/
u8 MCAL_GPIO_Read_Pin(GPIO_Port_t Port_Number,GPIO_Pin_Number_t Pin);

/********************************************************************
* Syntax          : MCAL_GPIO_Write_Pin(GPIOX_t *Port,GPIO_Pin_t Pin)
* Description     : Write Logic On Specific Pin
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(Copy To Pin Number "GPIO_Pin_Number_t"),(State "HI-LOW")
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void MCAL_GPIO_Write_Pin(GPIO_Port_t Port_Number,GPIO_Pin_Number_t Pin,u8 State);

/********************************************************************
* Syntax          : MCAL_GPIO_Toggle_Pin(GPIOX_t *Port,GPIO_Pin_t Pin)
* Description     : Toggle Specific Pin
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(Copy To Pin Number "GPIO_Pin_Number_t")
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void MCAL_GPIO_Toggle_Pin(GPIO_Port_t Port_Number,GPIO_Pin_Number_t Pin);

/********************************************************************
* Syntax          : MCAL_GPIO_Read_Port(GPIOX_t *Port)
* Description     : Read Logic State For Specific Port
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),
* Parameters (out): None
* Return value:   : u16 Refers To Port Value
********************************************************************/
u16 MCAL_GPIO_Read_Port(GPIO_Port_t Port_Number);

/********************************************************************
* Syntax          : MCAL_GPIO_Write_Port(GPIOX_t *Port)
* Description     : Write Logic On Specific Port
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),(u16 Value)
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void MCAL_GPIO_Write_Port(GPIO_Port_t Port_Number,u16 Value);

/********************************************************************
* Syntax          : MCAL_GPIO_Toggle_Port(GPIOX_t *Port)
* Description     : Toggle All Pins On Specific Port
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : (Ptr To The Port "GPIOX->X=A,B,C,D,E"),
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void MCAL_GPIO_Toggle_Port(GPIO_Port_t Port_Number);

/********************************************************************
 *  END OF FILE:  GPIO_Interface.h
********************************************************************/
#endif
