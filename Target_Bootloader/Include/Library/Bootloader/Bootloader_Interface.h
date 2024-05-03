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
#ifndef _BOOTLOADER_INTERFACE_H_
#define _BOOTLOADER_INTERFACE_H_
/******************************************************************/
/*****************************************
---------    Configure Macros    ---------
*****************************************/
//#define ENABLE_DEBUG
/* Version Flag Location In Flash */
#define Version_Location                ((const u32 *)(0x8007FFC))
/* Default Version */
#define Default_Chip_ID_Number          (250)
#define Default_SW_Major_Version        (250)
#define Default_SW_Minor_Version        (250)
/* Services Providded By Bootloader */
#define Total_Services                  (6)
/* Receive Buffer Size */
#define Maximum_Buffer_Size             (255)
/*****************************************
-----------    Define Macros    ----------
*****************************************/
/* Bootloader Pages */
#define Bootloader_Total_Pages          (32)
/* Application Pages */
#define Application_Page                (32)
/* Memory Base */
#define Memory_Base                     (0x8000000U)
/* Memory Size */
#define Memory_Size                     (0x10000U)
/* Reset Key */
#define SOFTWARE_RESET_KEY              ((0x5FA << 16) | (1 << 2))
/* Application Address */
#define Application_Base 			((volatile u32 *)((Memory_Base)+(Application_Page*1024)))
/* End Of Flash */
#define End_Memory 			          ((volatile u32 *)((Memory_Base)+(Memory_Size)))
/* Bootloader Address */
#define Bootloader_Base 			     ((volatile u32 *)(Memory_Base))
#define Valid_CRC                       (*((volatile u32 *)(0x8007FE0)))
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
void *memset(void *str, int c, unsigned int n);
/****************************************************************************************************
* Function Name   : Bootloader_Initialize
* Description     : Function to initialize the bootloader.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function initializes the UART and CRC modules used by the bootloader.
*****************************************************************************************************/
void Bootloader_Initialize(void);
/****************************************************************************************************
* Function Name   : Bootloader_Start
* Description     : Function to start the bootloader operation.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function is the entry point for the bootloader operation.
*                   - It continuously checks if the bootloader should be opened, the application should be started, or the bootloader should be interrupted.
*****************************************************************************************************/
void Bootloader_Start(void);
/********************************************************************
 *  END OF FILE:  Bootloader_Interface.h
********************************************************************/
#endif
