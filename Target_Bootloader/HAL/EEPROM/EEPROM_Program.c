/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  EEPROM_Program.c
 *  Module:  HAL/EEPROM
 *  Description:  EEPROM Driver Program File
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "EEPROM_Interface.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/			                                                                   /*Initial Value For The Key*/
static I2C_Config_t I2C_1_Configuration=I2C_Congiguration;
/********************************************************************
* Syntax          : EEPROM_Initialization(void)
* Description     : Initialize EEPROM With I2C 1 Configuration Configurations
* Sync-Async      : *
* Reentrancy      : *
* Parameters (in) : None
* Parameters (out): None
* Return value:   : Void
********************************************************************/
void EEPROM_Initialization(void)
{
     I2C_Initialization(&I2C_1_Configuration);
}
/********************************************************************
* Function          : EEPROM_Write(u16 Address, u8 *Data, u8 Size)
* Description       : Write data to the EEPROM at the specified address.
* Parameters (in)   :  Address - The EEPROM address to write to.
                       Data - A pointer to the data to write.
                       Size - The size of the data to write.
* Parameters (out)  : None
* Return value      : Void
********************************************************************/
void EEPROM_Write(u16 Address,u8 *Data,u8 Size)
{
     u8 Counter,*Ptr=(u8*)&Address;Size--;
     I2C_Send_Receive(&I2C_1_Configuration,Memory_Address,Ptr+ONE,I2C_Write,I2C_Start);
     I2C_Send_Receive(&I2C_1_Configuration,Memory_Address,Ptr,I2C_Write,I2C_Data);
     for(Counter=ZERO;Counter<Size;Counter++)I2C_Send_Receive(&I2C_1_Configuration,Memory_Address,&Data[Counter],I2C_Write,I2C_Data);
     I2C_Send_Receive(&I2C_1_Configuration,Memory_Address,&Data[Counter],I2C_Write,I2C_Stop);
}
/********************************************************************
* Function          : EEPROM_Read(u16 Address, u8 *Data, u8 Size)
* Description       : Read data from the EEPROM from the specified address.
* Parameters (in)   :  Address - The EEPROM address to read from.
                       Data - A pointer to the buffer to store the read data.
                       Size - The size of the data to read.
* Parameters (out)  : Data - A pointer to the buffer containing the read data.
* Return value      : Void
********************************************************************/
void EEPROM_Read(u16 Address,u8 *Data,u8 Size)
{
     u8 Counter,*Ptr=(u8*)&Address;Size--;
     I2C_Send_Receive(&I2C_1_Configuration,Memory_Address,Ptr+ONE,I2C_Write,I2C_Start);
     I2C_Send_Receive(&I2C_1_Configuration,Memory_Address,Ptr,I2C_Write,I2C_Data);
     I2C_Send_Receive(&I2C_1_Configuration,Memory_Address,&Data[ZERO],I2C_Read,I2C_Start);
     for(Counter=ONE;Counter<Size;Counter++)I2C_Send_Receive(&I2C_1_Configuration,Memory_Address,&Data[Counter],I2C_Read,I2C_Data);
     I2C_Send_Receive(&I2C_1_Configuration,Memory_Address,&Data[Counter],I2C_Read,I2C_Stop);
}
/********************************************************************
 *  END OF FILE:  EEPROM_Program.c
********************************************************************/
