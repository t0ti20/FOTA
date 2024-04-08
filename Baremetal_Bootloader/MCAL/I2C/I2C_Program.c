/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  I2C_Program.c
 *  Module:  MCAL/I2C
 *  Description:  Intra-Integrated Circuit (TWI) driver implementation
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "I2C_Interface.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/********************************************************************
 * Function      : void I2C_Initialization(I2C_Config_t *I2C_Config)
 * Description   : Initializes the I2C peripheral based on the I2C configuration passed as a parameter
 * Sync-Async    : *
 * Reentrancy    : *
 * Parameters    : (I2C_Config_t *I2C_Config) - pointer to the I2C configuration struct
 * Return value  : None
 ********************************************************************/
void I2C_Initialization(I2C_Config_t *I2C_Config)
{
     u64 Module_Clock;
     I2C_t *I2CX;
     GPIO_Pin_Config_t Pin = {Pin_6, Alternate_Output_Open_Drain, Output_10};
     GPIO_Pin_Number_t SCL = Pin_6, SDA = Pin_7;
     /*Get Module Clock*/
     Module_Clock = RCC_Get_APB1_Clock() / 0xF4240;
     /*Select Which I2C And Enable Clock*/
     if (I2C_Config->Number == I2C_1)
     {
          I2CX = &I2C1;SCL = Pin_6;SDA = Pin_7;RCC_Clock(RCC_I2C1, Enable);
     }
     else
     {
          I2CX = &I2C2;SCL = Pin_10;SDA = Pin_11;RCC_Clock(RCC_I2C2, Enable);
     }
     /*Initialize GPIO Pins*/
     Pin.Pin_Number = SCL;
     MCAL_GPIO_Initialize(GPIO_B, Pin);
     Pin.Pin_Number = SDA;
     MCAL_GPIO_Initialize(GPIO_B, Pin);
     /*Configure Peripheral Clock Frequency*/
     Write_Bits(I2CX->CR2.Register, ZERO, SIX, Module_Clock);
     /*Configure The Clock Control Registers CCR*/
     Write_Bits(I2CX->CCR.Register, ZERO, 12, (I2C_Config->Speed / (0x3E8 / Module_Clock)));
     /*Select SM Mode I2C master mode selection*/
     I2CX->CCR.Bits.Bit_15 = Disable;
     /*Configure The Rise Time Register*/
     Write_Bits(I2CX->TRISE.Register, ZERO, SIX, Module_Clock + ONE);
     /*Configure Address*/
     I2CX->OAR1.Bits.Bit_15 = Disable;
     Write_Bits(I2CX->OAR1.Register, ONE, SEVEN,I2C_Config->Address);
     /*Configure Interrupts*/
     /*============================= TO DO =============================*/
     /*Enable Auto ACK*/
     I2C_Generate_ACK;
     /*Clock Stretching Disable (Slave mode)*/
     I2CX->CR1.Bits.Bit_7=Disable;
     /*Enable Peripheral*/
     I2CX->CR1.Bits.Bit_0 = Enable;
}
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
void I2C_Send_Receive(I2C_Config_t *I2C_Config,u8 Address,u8 *Data,I2C_Operation_t Operation,I2C_State_t State)
{
     static logic_t Flag=I2C_F_Bus_Not_Owned;
     I2C_t *I2CX;
     /* Select Which I2C */
     I2CX=(I2C_Config->Number==I2C_1)?&I2C1:&I2C2;
     /* Check For State */
     if(State==I2C_Start||State==I2C_Start_Stop)
     {
          /* Check If I Own The Bus */
          if(Flag!=I2C_F_Bus_Owned)
          {
               /* Wait Till Line Free */
               if(State==I2C_Start)while(I2C_F_Busy){}
               Flag=I2C_F_Bus_Owned;
          }
          /* Generate Start Bit */
          I2C_Generate_Start;
          /* Wait For Start Bit To Be Generated */
          while(!I2C_F_Start_Bit_Sent){}
          /* Write The Address in DR register */
          I2CX->DR.Register=(u32)(Address<<ONE)|Operation;
          /* Wait For Address To Find ADDR */ 
          while (!I2C_F_Address_Found){}
          /* Reset Buffer */
          if(I2CX->SR2.Register){}
     }
     if(Operation==I2C_Write)
     {
		/* Wait Till Shift Register To Be Empty */
          while (!I2C_F_Transimitted){}
          /* Write Data */
          I2CX->DR.Register=*Data;
          /* Wait For Acknowlage */
          while(!I2C_F_Transimitted){}
     }
     /* Read Data */
     else 
     {
          /* Wait Till Shift Register To Have Data */
          while (!I2C_F_Receved){}
          /* Read Data */
          *Data=(u8)I2CX->DR.Register;
     }
     /* Stop Condition */
     if(State==I2C_Stop||State==I2C_Start_Stop)
     {
          Flag=I2C_F_Bus_Not_Owned;
          I2C_Generate_Stop;
     }
}
/********************************************************************
 *  END OF FILE:  I2C_Program.c
 ********************************************************************/
