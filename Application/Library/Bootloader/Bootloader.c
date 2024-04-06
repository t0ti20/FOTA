/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  Bootloader.c
 *  Module:  Library/Bootloader
 *  Description:  Bootloader Logic File
*******************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "Bootloader.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
static u8 Chip_ID=Chip_ID_Number;
static u8 UART_Buffer[Maximum_Buffer_Size];
static u8 SW_Major_Version=Basic_SW_Major_Version;
static u8 SW_Minor_Version=Basic_SW_Minor_Version;
static USART_Config_t UART_ONE={NULL,USART_1,USART_No_Parity,USART_Rx_Tx,Disable,USART_Eight_Bits,USART_One_Stop,USART_115200,USART_Disable_Interrupt};
/*****************************************
---  Application Programming Interface  --
*****************************************/
static void Bootloader_Send_ACK(u8 Data_Length)
{		
	#ifdef ENABLE_DEBUG
	Bootloader_Send_Message("Sending Ack + Size Of %d Bytes\n",Data_Length);
	#endif
	USART_Transmit(&UART_ONE,(u16)Bootloader_State_ACK);
	USART_Transmit(&UART_ONE,(u16)Data_Length);
}
static void Bootloader_Send_NACK(void)
{
	USART_Transmit(&UART_ONE,(u16)Bootloader_State_NACK);
}

static logic_t Bootloader_CRC_Check(void)
{
	u32 Actual_CRC=ZERO;
	u32 Value=ZERO;
	CRC_Reset();
    /* Accumulate CRC over the data bytes (excluding CRC bytes) */
    for(u8 Buffer_Counter=ZERO;Buffer_Counter<UART_Buffer[ZERO]-THREE;Value=ZERO)
    {
        /* Extract 32-bit word from buffer */
        for (u8 Ward_Counter=ZERO;(Ward_Counter<FOUR)&&(Buffer_Counter<UART_Buffer[ZERO]-THREE);Ward_Counter++,Buffer_Counter++)
		{
			Value|=(UART_Buffer[Buffer_Counter]<<EIGHT*((THREE-Ward_Counter)));
		}
        /* Accumulate CRC over the 32-bit word */
        Actual_CRC=CRC_Accumulate(Value);
    }
	return (*((u32*)(UART_Buffer+(UART_Buffer[ZERO]-THREE))))==Actual_CRC?TRUE:FALSE;
}
static void Bootloader_Send_Version(void)
{
	u8 Message[3]={Chip_ID,SW_Major_Version,SW_Minor_Version};
	Bootloader_Send_ACK(THREE);
	#ifdef ENABLE_DEBUG
	Bootloader_Send_Message("Done Sending 3 Elements Of Vesrion\n");
	#endif
	USART_Send_Array(&UART_ONE,Message,THREE);
}
static void Bootloader_Erase_Flash(void)
{
	Bootloader_Send_ACK(ONE);
	if(Flash_State_Ok==Flash_Erase_Pages(Bootloader_Total_Pages,(128-Bootloader_Total_Pages)))
	{
		USART_Transmit(&UART_ONE,Bootloader_State_Successful_Erase);
	}
	else
	{
		USART_Transmit(&UART_ONE,Bootloader_State_Unsuccessful_Erase);
	}
}

static void Bootloader_Send_ID(void)
{
	volatile u32 *Variable=&UNIQUE_DEVICE_ID_REGISTER;
	Bootloader_Send_ACK(12);
	USART_Send_Array(&UART_ONE,(u8*)Variable,12);
}

static void Bootloader_Send_Help(void)
{
	u8 Message[Total_Services]={Bootloader_Command_Send_Help,Bootloader_Command_Send_ID,Bootloader_Command_Send_Version,Bootloader_Command_Erase_Flash,Bootloader_Command_Write_Flash};
	Bootloader_Send_ACK(Total_Services);
	USART_Send_Array(&UART_ONE,Message,Total_Services);
	#ifdef ENABLE_DEBUG
	Bootloader_Send_Message("Done Sending Help\n");
	#endif
} 

static void Bootloader_Write_Flash(void)
{
	Bootloader_Send_ACK(ONE);
	
	if(Bootloader_State_OK==Flash_Write_Pages(Bootloader_Total_Pages,(u16*)(&UART_Buffer[TWO]),UART_Buffer[TWO]-FIVE))
	{
		USART_Transmit(&UART_ONE,Bootloader_State_Successful_Write);
	}
	else
	{
		USART_Transmit(&UART_ONE,Bootloader_State_Unsuccessful_Write);
	}
}

static Bootloader_State_t Bootloader_Check_Command(void)
{
	Bootloader_State_t Return=Bootloader_State_OK;
	switch ((Bootloader_Command_t)UART_Buffer[1])
	{
		case Bootloader_Command_Send_Version:Bootloader_Send_Version();break;
		case Bootloader_Command_Send_Help:Bootloader_Send_Help();break;
		case Bootloader_Command_Send_ID:Bootloader_Send_ID();break;
		case Bootloader_Command_Erase_Flash:Bootloader_Erase_Flash();break;
		case Bootloader_Command_Write_Flash:Bootloader_Write_Flash();break;
		default : Return=Bootloader_State_Wrong_Command;break;
	}
	return Return;
}

Bootloader_State_t Bootloader_Receive_Command(void)
{
	Bootloader_State_t Return=Bootloader_State_OK;
	u8 Frame_Length=ZERO;
	/* Receive Frame Size */
	UART_Buffer[ZERO]=(u8)USART_Receive(&UART_ONE);
	/* Receive Whole Frame */
	for(Frame_Length=ONE;Frame_Length<=UART_Buffer[ZERO];Frame_Length++)
	{
		UART_Buffer[Frame_Length]=(u8)USART_Receive(&UART_ONE);
	}
	#ifdef ENABLE_DEBUG
	Bootloader_Send_Message("Recevig Done\n");
	#endif
	if(TRUE==Bootloader_CRC_Check())
	{
		#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("CRC Passed\n");
		#endif
		Return=Bootloader_Check_Command();
	}
	else
	{
		#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("CRC Failed\n");
		#endif
		Bootloader_Send_NACK();
	}
	return Return;
}

void Bootloader_Send_Message(const u8 *Message,...)
{
	u8 Message_Buffer[Maximum_Buffer_Size]={};
	va_list Arguments;
	va_start(Arguments,Message);
	vsprintf((char *)Message_Buffer,(const char *)Message,Arguments);
	USART_Send_String(&UART_ONE,(const u8 *)Message_Buffer);
	va_end(Arguments);
}

void Bootloader_Initialize(void)
{
	USART_Initialization(&UART_ONE);
	CRC_Initialization();
}
/********************************************************************
 *  END OF FILE:  Bootloader.c
********************************************************************/