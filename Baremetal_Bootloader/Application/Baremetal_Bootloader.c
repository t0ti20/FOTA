/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  Application.c
 *  Module:  Application
 *  Description:  Main Application Logic File
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "Baremetal_Bootloader.h"
void Test(void);
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
int main(void) 
{
	System_Initialization();
	while (1)
	{
		Test();
		Bootloader_Start();
	}
}
/****************************************/
void Test(void)
{
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_14,1);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_15,1);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_0,1);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_1,1);
}
void System_Initialization(void)
{
    GPIO_Pin_Config_t Pin={Pin_14,Output_Push_Pull,Output_10};
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_15;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_0;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_1;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Bootloader_Initialize();
}
/********************************************************************
 *  END OF FILE:  Application.c
********************************************************************/