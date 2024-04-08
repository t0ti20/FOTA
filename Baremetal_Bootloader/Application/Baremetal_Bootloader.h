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
void System_Initialization(void);
/********************************************************************
 *  END OF FILE:  Application.h
********************************************************************/
#endif
