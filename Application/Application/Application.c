/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Author: Khaled El-Sayed @t0ti20
 *  File: ./Application.c
 *  Date: April 8, 2024
 *  Description: Simple Blink Application Illustrating Bootloader
 *  (C) 2023 "@t0ti20". All rights reserved.
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "Application.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/*****************************************************************************************
* Function Name   : main
* Description     : Entry point of the program. Initializes the system and continuously 
*                   executes Test_Blue function in an infinite loop.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Integer (Exit status of the program)
*****************************************************************************************/
int main(void) 
{	
	System_Initialization();
	while (1)
	{
		//Test_Blue();
		//Test_Yellow();;
	}
}
/*****************************************
--------------    APIs     ---------------
*****************************************/
/*****************************************************************************************
* Function Name   : Test_Yellow
* Description     : Performs a test by toggling specific GPIO pins and delaying for a 
*                   specified time.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Test_Yellow(void)
{
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_14,1);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_15,1);
	delay_ms(DELAY);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_14,0);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_15,0);
	delay_ms(DELAY);
	delay_ms(DELAY);
}
/*****************************************************************************************
* Function Name   : Test_Blue
* Description     : Performs a test by toggling GPIO pins and delaying for a specified time.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Test_Blue(void)
{
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_0,1);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_1,1);
	delay_ms(DELAY);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_0,0);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_1,0);
	delay_ms(DELAY);
	delay_ms(DELAY);
}
/*****************************************************************************************
* Function Name   : System_Initialization
* Description     : Initializes the system by configuring external interrupts, LED pins,
*                   and setting LED pins to low.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void System_Initialization(void)
{
	/* Initialize External Interrupt */
	EXTI_Config_t Bootloader={.Port=GPIO_A,.Pin=Pin_8,.Sensing_Edge=Faling_Edge,.Interrupt=Enable,.Event=Disable,.Ptr_Function=Jump_To_Bootloader};
    /* Initialize Led Pins */
	GPIO_Pin_Config_t Pin={Pin_14,Output_Push_Pull,Output_10};
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_15;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_0;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
	Pin.Pin_Number=Pin_1;
	MCAL_GPIO_Initialize(GPIO_B,Pin);
    /* Set Led Pins To Low */
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_0,0);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_1,0);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_14,0);
	MCAL_GPIO_Write_Pin(GPIO_B,Pin_15,0);
	/* Initialize Bootloader Jump As External Interrupt */
	EXTI_Initilization(Bootloader);
	/* Bootloader Version */
	Bootloader_Set_Application_Version(CHIP_ID,MAJOR,MINOR);
}
/*****************************************************************************************
* Function Name   : Jump_To_Bootloader
* Description     : Jumps to the bootloader.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Jump_To_Bootloader(void)
{
	Bootloader_Jump();
}
/********************************************************************
 *  END OF FILE:  Application.c
********************************************************************/
