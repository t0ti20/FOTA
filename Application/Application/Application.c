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
#include "GPIO_Interface.h"
void Test_1(void);
void Test_2(void);
void System_Initialization(void);
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
int main(void) 
{	
	System_Initialization();
	while (1)
	{
		Test_1();
	}
}
/****************************************/
void Test_1(void)
{
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_14,1);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_15,1);
	delay_ms(1000);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_14,0);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_15,0);
	delay_ms(1000);
	delay_ms(1000);
}
void Test_2(void)
{
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_0,1);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_1,1);
	delay_ms(1000);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_0,0);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_1,0);
	delay_ms(1000);
	delay_ms(1000);
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
}
/********************************************************************
 *  END OF FILE:  Application.c
********************************************************************/
