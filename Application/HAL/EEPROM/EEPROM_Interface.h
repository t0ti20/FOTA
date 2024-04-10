/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  EEPROM_Interface.h
 *  Module:  HAL/EEPROM
 *  Description:  EEPROM Driver Interface File
*******************************************************************/
#ifndef _EEPROM_INTERFACE_H_
#define _EEPROM_INTERFACE_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "I2C_Interface.h"
/*****************************************
--------    Type  Definitions    ---------
*****************************************/

/*****************************************
---------   Prebuild Configure   ---------
*****************************************/
#define I2C_Connection			(I2C_1)
#define I2C_MCU_Address			(0x11)
#define Memory_Address             (0x2A)
/*****************************************
---------    Configure Macros    ---------
*****************************************/
#define I2C_Congiguration		{NULL,I2C_Connection,I2C_100K,I2C_Stretching_Enabled,I2C_Default,I2C_MCU_Address}
/*****************************************
---  Application Programming Interface  --
*****************************************/
/********************************************************************
* Function          : EEPROM_Initialization(void)
* Description       : Initialize EEPROM with pre-built configurations.
* Sync-Async        : *
* Reentrancy        : *
* Parameters (in)   : None
* Parameters (out)  : None
* Return value      : Void
********************************************************************/
void EEPROM_Initialization(void);
/********************************************************************
* Function          : EEPROM_Write(u16 Address, u8 *Data, u8 Size)
* Description       : Write data to the EEPROM at the specified address.
* Parameters (in)   : Address - The EEPROM address to write to.
                       Data - A pointer to the data to write.
                       Size - The size of the data to write.
* Parameters (out)  : None
* Return value      : Void
********************************************************************/
void EEPROM_Write(u16 Address,u8 *Data,u8 Size);
/********************************************************************
* Function          : EEPROM_Read(u16 Address, u8 *Data, u8 Size)
* Description       : Read data from the EEPROM from the specified address.
* Parameters (in)   : Address - The EEPROM address to read from.
                       Data - A pointer to the buffer to store the read data.
                       Size - The size of the data to read.
* Parameters (out)  : Data - A pointer to the buffer containing the read data.
* Return value      : Void
********************************************************************/
void EEPROM_Read(u16 Address,u8 *Data,u8 Size);
/********************************************************************
 *  END OF FILE:  EEPROM_Interface.h
********************************************************************/
#endif
