/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Author: Khaled El-Sayed @t0ti20
 *  File: ./Bootloader_Interface.h
 *  Date: April 8, 2024
 *  Module:  Library/Bootloader
 *  Description: Bootloader Interface File
 *  (C) 2023 "@t0ti20". All rights reserved.
*******************************************************************/
#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_
/******************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "CRC.h"
#include "FLASH.h"
#include "USART_Interface.h"
/*****************************************
---------    Configure Macros    ---------
*****************************************/
//#define ENABLE_DEBUG
#define Version_Location                ((const u32 *)(0x8007FFc))
#define Default_Chip_ID_Number          (250)
#define Default_SW_Major_Version        (250)
#define Default_SW_Minor_Version        (250)
#define Total_Services                  (6)
#define Maximum_Buffer_Size             (255)
/*****************************************
-----------    Define Macros    ----------
*****************************************/
#define Bootloader_Total_Pages          (32)
#define Application_Page                (32)
#define Memory_Base                     (0x8000000U)
#define Memory_Size                     (0x20000U)
#define SOFTWARE_RESET_KEY              ((0x5FA << 16) | (1 << 2))
#define Application_Base 			((volatile u32 *)((Memory_Base)+(Application_Page*1024)))
#define Bootloader_Base 			     ((volatile u32 *)(Memory_Base))
/*****************************************
-------    Macro Like Function    --------
*****************************************/
#define SET_VERSION(ID,MAJOU,MINOR)     (ID<<24)|(MAJOU<<16)|(MINOR<<8)
#define SET_MSP(Task_Address)            __asm volatile ("MSR MSP,%[Variable]"::[Variable]"r"(Task_Address))
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
/*-------------- Send State ------------*/
typedef enum Bootloader_State_t
{
     Bootloader_State_OK                     =(0),
     Bootloader_State_ACK                    =(1),
     Bootloader_State_NACK                   =(2),
     Bootloader_State_Successful_Erase       =(3),
     Bootloader_State_Unsuccessful_Erase     =(4),
     Bootloader_State_Successful_Write       =(5),
     Bootloader_State_Unsuccessful_Write     =(6),
     Bootloader_State_Error_CRC              =(7),
     Bootloader_State_Wrong_Command          =(8)
}Bootloader_State_t;
/*---------------- Command -------------*/
typedef enum Bootloader_Command_t
{    
     Bootloader_Command_Send_Help            =(1),
     Bootloader_Command_Send_ID              =(2),
     Bootloader_Command_Send_Version         =(3),
     Bootloader_Command_Erase_Flash          =(4),
     Bootloader_Command_Flash_Application    =(5),
     Bootloader_Command_Address_Jump         =(6),
     Bootloader_Command_Say_Hi               =(7),
     Bootloader_Command_Say_Bye              =(8),
     Bootloader_Command_Send_Data            =(9)
}Bootloader_Command_t;
/*****************************************
---  Application Programming Interface  --
*****************************************/
/*****************************************************************************************
* Function Name   : Bootloader_Initialize
* Description     : Initializes necessary modules for the bootloader operation.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Bootloader_Initialize(void);
/*****************************************************************************************
* Function Name   : Bootloader_Start
* Description     : Starts the bootloader application. It checks for a flag in the flash 
*                   memory. If the flag is set, it waits to receive a command from the 
*                   host. Otherwise, it starts the flashed application.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Bootloader_Start(void);
/*****************************************************************************************
* Function Name   : Bootloader_Jump
* Description     : Initiates a jump from the current application to the bootloader 
*                   application. This function erases a flag to ensure the system stays 
*                   in bootloader mode during the next boot, and then jumps to the 
*                   bootloader application.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
//void Bootloader_Jump(void);

void Bootloader_Set_Application_Version(u8 ID,u8 Major,u8 Minor);

/********************************************************************
 *  END OF FILE:  Bootloader_Interface.h
********************************************************************/
#endif
