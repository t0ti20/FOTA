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
#include "Application.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
int main(void) 
{	
	System_Initialization();
	while (1)
	{
		Test_2();
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
/****************************************/
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
/****************************************/
void System_Initialization(void)
{
	EXTI_Config_t Bootloader={.Port=GPIO_A,.Pin=Pin_8,.Sensing_Edge=Faling_Edge,.Interrupt=Enable,.Event=Disable,.Ptr_Function=Jump_To_Bootloader};
    GPIO_Pin_Config_t Pin={Pin_14,Output_Push_Pull,Output_10};
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_15;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_0;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_1;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_0,0);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_1,0);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_14,0);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_15,0);
	EXTI_Initilization(Bootloader);
}
/****************************************/
void Jump_To_Bootloader(void)
{
	Bootloader_Jump();
}
/********************************************************************
 *  END OF FILE:  Application.c
********************************************************************/
