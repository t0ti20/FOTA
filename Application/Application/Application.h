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
#include "GPIO_Interface.h"
#include "CRC.h"
#include "Macros.h"
#include "FLASH.h"
#include "Bootloader.h"
/*****************************************
---  Application Programming Interface  --
*****************************************/
void System_Initialization(void);
/********************************************************************
 *  END OF FILE:  Application.h
********************************************************************/
#endif
