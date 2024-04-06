/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  Bootloader.h
 *  Module:  Library/Bootloader
 *  Description:  Bootloader Interface File
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
#define Chip_ID_Number                  (123)
#define Basic_SW_Major_Version          (0)
#define Basic_SW_Minor_Version          (0)
#define Maximum_Buffer_Size             (250)
#define Bootloader_Total_Pages          (32)
#define Total_Services                  (5)
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
/*-------------- Send State ------------*/
typedef enum Bootloader_State_t
{
     Bootloader_State_OK                =(0),
     Bootloader_State_ACK               =(1),
     Bootloader_State_NACK              =(2),
     Bootloader_State_Successful_Erase  =(3),
     Bootloader_State_Unsuccessful_Erase=(4),
     Bootloader_State_Successful_Write  =(5),
     Bootloader_State_Unsuccessful_Write=(6),
     Bootloader_State_Wrong_Command     =(7)
}Bootloader_State_t;
/*---------------- Command -------------*/
typedef enum Bootloader_Command_t
{    
     Bootloader_Command_Send_Help       =(1),
     Bootloader_Command_Send_ID         =(2),
     Bootloader_Command_Send_Version    =(3),
     Bootloader_Command_Erase_Flash     =(4),
     Bootloader_Command_Write_Flash     =(5)
}Bootloader_Command_t;
/*****************************************
---  Application Programming Interface  --
*****************************************/
void Bootloader_Initialize(void);
Bootloader_State_t Bootloader_Receive_Command(void);
void Bootloader_Send_Message(const u8 Message[],...);
/********************************************************************
 *  END OF FILE:  Bootloader.h
********************************************************************/
#endif
