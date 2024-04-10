/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Author: Khaled El-Sayed @t0ti20
 *  File: ./Application.h
 *  Date: April 8, 2024
 *  Description: Simple Blink Application Illustrating Bootloader Headder File
 *  (C) 2023 "@t0ti20". All rights reserved.
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
---------    Configure Macros    ---------
*****************************************/
#define CHIP_ID                         (123)
#define MAJOR                           (1)
#define MINOR                           (2)
/*****************************************
---  Application Programming Interface  --
*****************************************/
/*****************************************************************************************
* Function Name   : Test_Yellow
* Description     : Performs a test by toggling specific GPIO pins and delaying for a 
*                   specified time.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Test_Yellow(void);
/*****************************************************************************************
* Function Name   : Test_Blue
* Description     : Performs a test by toggling GPIO pins and delaying for a specified time.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Test_Blue(void);
/*****************************************************************************************
* Function Name   : System_Initialization
* Description     : Initializes the system by configuring external interrupts, LED pins,
*                   and setting LED pins to low.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void System_Initialization(void);
/*****************************************************************************************
* Function Name   : Jump_To_Bootloader
* Description     : Jumps to the bootloader.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Jump_To_Bootloader (void);
/********************************************************************
 *  END OF FILE:  Application.h
********************************************************************/
#endif
