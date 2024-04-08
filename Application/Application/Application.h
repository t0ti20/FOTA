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
#include "EXTI_Interface.h"
#include "Bootloader_Interface.h"
/*****************************************
---  Application Programming Interface  --
*****************************************/
void Test_1(void);
void Test_2(void);
void System_Initialization(void);
void Jump_To_Bootloader (void);
/********************************************************************
 *  END OF FILE:  Application.h
********************************************************************/
#endif
