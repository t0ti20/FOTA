/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  Application.h
 *  Module:  Application
 *  Description:  Application Interface File
*******************************************************************/
#ifndef _APPLICATION_H_
#define _APPLICATION_H_
/********************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "Bootloader_Interface.h"
#include "GPIO_Interface.h"
#include "Macros.h"
#include "FLASH.h"
#include "CRC.h"
/*****************************************
---  Application Programming Interface  --
*****************************************/
/*****************************************************************************************
* Function Name   : System_Initialization
* Description     : Initializes the system components including LED pins and the bootloader.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void System_Initialization(void);
/*****************************************************************************************
* Function Name   : Test
* Description     : Tests the application by indicating that the bootloader is running.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Test(void);
/********************************************************************
 *  END OF FILE:  Application.h
********************************************************************/
#endif
