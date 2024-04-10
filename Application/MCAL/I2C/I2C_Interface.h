/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  I2C_Program.c
 *  Module:  MCAL/I2C
 *  Description:  Intra-Integrated Circuit (TWI) driver implementation
*******************************************************************/
#ifndef _I2C_INTERFACE_H_
#define _I2C_INTERFACE_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "STM32F103.h"
#include "RCC_Interface.h"
#include "NVIC_Interface.h"
#include "GPIO_Interface.h"
/*****************************************
------------    Flag Macros    ------------
*****************************************/
#define I2C_Generate_Start              I2CX->CR1.Bits.Bit_8 = Enable
#define I2C_Generate_Stop               I2CX->CR1.Bits.Bit_9 = Enable
#define I2C_Generate_ACK                I2CX->CR1.Bits.Bit_10 = Enable
#define I2C_Generate_NACK               I2CX->CR1.Bits.Bit_10 = Enable
#define I2C_F_Busy                      I2CX->SR2.Bits.Bit_1
#define I2C_F_Start_Bit_Sent            I2CX->SR1.Bits.Bit_0
#define I2C_F_Address_Found             I2CX->SR1.Bits.Bit_1
#define I2C_F_Data_Transmitted          I2CX->SR2.Bits.Bit_2
#define I2C_F_Master                    I2CX->SR2.Bits.Bit_0
#define I2C_F_Transimitted              I2CX->SR1.Bits.Bit_7
#define I2C_F_Receved                   I2CX->SR1.Bits.Bit_6
#define I2C_F_Bus_Owned                 Enable
#define I2C_F_Bus_Not_Owned             Disable
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
/*------------- I2C Modes --------------*/
typedef enum I2C_Stretching_t
{
     I2C_Stretching_Enabled              =(0),
     I2C_Stretching_Disabled             =(1)
}I2C_Stretching_t;
/*------------- I2C Speed --------------*/
typedef enum I2C_Speed_t
{
     I2C_100K                            =(5000),
     I2C_50K                             =(10000)
}I2C_Speed_t;
/*------------- I2C Modes --------------*/
typedef enum I2C_Mode_t
{
     I2C_Default                         =(0),
     I2C_SMBus                           =(1)
}I2C_Mode_t;
/*------------- I2C Modes --------------*/
typedef enum I2C_Number_t
{
     I2C_1                               =(0),
     I2C_2                               =(1)
}I2C_Number_t;
/*------------- I2C Modes --------------*/
typedef enum I2C_Operation_t
{
     I2C_Write                           =(0),
     I2C_Read                            =(1)
}I2C_Operation_t;
/*------------- I2C State --------------*/
typedef enum I2C_State_t
{//Start//STOP
     I2C_Data                            =(0),
     I2C_Stop                            =(1),
     I2C_Start                           =(2),
     I2C_Start_Stop                      =(3),
}I2C_State_t;
/*------------- I2C Config -------------*/
typedef struct __attribute__((packed)) I2C_Config_t
{
     void(*Call_Back_Function)(void);
     I2C_Number_t Number;
     I2C_Speed_t Speed;
     I2C_Stretching_t Time_Stretching;
     I2C_Mode_t Mode;
     u8 Address;
} I2C_Config_t;
/*****************************************
---  Application Programming Interface  --
*****************************************/
/********************************************************************
 * Function      : void I2C_Initialization(I2C_Config_t *I2C_Config)
 * Description   : Initializes the I2C peripheral based on the I2C configuration passed as a parameter
 * Sync-Async    : *
 * Reentrancy    : *
 * Parameters    : (I2C_Config_t *I2C_Config) - pointer to the I2C configuration struct
 * Return value  : None
 ********************************************************************/
void I2C_Initialization(I2C_Config_t *I2C_Config);
/********************************************************************
 * Function      : void I2C_Send_Receive(I2C_Config_t *I2C_Config, u8 Address, u8 *Data, I2C_Operation_t Operation, I2C_State_t State)
 * Description   : Sends or receives data over I2C based on the operation and state passed as parameters
 * Sync-Async    : *
 * Reentrancy    : *
 * Parameters    : (I2C_Config_t *I2C_Config) - pointer to the I2C configuration struct
                   (u8 Address) - I2C address to communicate with
                   (u8 *Data) - pointer to data buffer to send/receive
                   (I2C_Operation_t Operation) - operation to perform (send/receive)
                   (I2C_State_t State) - state of the I2C bus (start, stop, start-stop)
 * Return value  : None
 ********************************************************************/
void I2C_Send_Receive(I2C_Config_t *I2C_Config,u8 Address,u8 *Data,I2C_Operation_t Operation,I2C_State_t State);
/********************************************************************
 *  END OF FILE:  I2C_Interface.h
 ********************************************************************/
#endif
