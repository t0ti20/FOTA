/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Author: Khaled El-Sayed @t0ti20
 *  File: ./Baremetal_Bootloader.c
 *  Date: April 8, 2024
 *  Description: Application Logic File For Bootloader
 *  (C) 2023 "@t0ti20". All rights reserved.
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "Baremetal_Bootloader.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/*****************************************************************************************
* Function Name   : main
* Description     : The entry point of the program. Initializes the system, runs the 
*                   application test, and starts the bootloader.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Integer representing the exit status of the program.
*****************************************************************************************/
int main(void) 
{
	System_Initialization();
	while (1)
	{
		Test();
		Bootloader_Start();
	}
}
/*****************************************
--------------    APIs     ---------------
*****************************************/
/*****************************************************************************************
* Function Name   : Test
* Description     : Tests the application by indicating that the bootloader is running.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Test(void)
{
	/* Test Application Indicate Bootloader Is Running */
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_14,1);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_15,1);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_0,1);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_1,1);
}
/*****************************************************************************************
* Function Name   : System_Initialization
* Description     : Initializes the system components including LED pins and the bootloader.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void System_Initialization(void)
{
	/* Initialize Led Pins */
    GPIO_Pin_Config_t Pin={Pin_14,Output_Push_Pull,Output_10};
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_15;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_0;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_1;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	/* Initialize Bootloader */
	Bootloader_Initialize();
}
/********************************************************************
 *  END OF FILE:  Application.c
********************************************************************/