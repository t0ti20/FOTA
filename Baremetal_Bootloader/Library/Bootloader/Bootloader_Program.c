/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Author: Khaled El-Sayed @t0ti20
 *  File: ./Bootloader_Program.c
 *  Date: April 8, 2024
 *  Module:  Library/Bootloader
 *  Description: Bootloader Program File
 *  (C) 2023 "@t0ti20". All rights reserved.
*******************************************************************/
/*****************************************
----------   Include Files    ------------
*****************************************/
#include "Bootloader_Interface.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************
/* UART Setting */
static USART_Config_t UART_ONE={NULL,USART_1,USART_No_Parity,USART_Rx_Tx,Disable,USART_Eight_Bits,USART_One_Stop,USART_115200,USART_Disable_Interrupt};
/* Major Version */
static const u8 SW_Major_Version=Basic_SW_Major_Version;
/* Minor Version */
static const u8 SW_Minor_Version=Basic_SW_Minor_Version;
/* Chip Configured ID */
static const u8 Chip_ID=Chip_ID_Number;
/* UART Buffer */
static u8 UART_Buffer[Maximum_Buffer_Size];
/*****************************************
-----------  Static Functions  -----------
*****************************************/
/*****************************************************************************************
* Function Name   : Bootloader_Start_Application
* Description     : Starts the user application by setting the stack pointer and jumping 
*                   to the application reset handler.
* Parameters (in) : Application_Address - Pointer to the address of the user application.
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
static void Bootloader_Start_Application(volatile u32 *Application_Address)
{
	/* Pointer To Application Reset Handlar */
	void (*Application_Reset)(void);
	/* Set Application Stack Pointer */
	SET_MSP(Application_Address[0]);
	/* Set Application Reset Handler */
	Application_Reset=(void *)Application_Address[1];
	/* TODO : Deinitialization Of All Modules */
	/* Debug Information */
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Starting User Application\n",Data_Length);
	#endif
	/* Jump To Application */
	Application_Reset();
}

/*****************************************************************************************
* Function Name   : Bootloader_Send_ACK
* Description     : Sends an acknowledgment (ACK) message over UART.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
static void Bootloader_Send_ACK()
{		
	/* Send ACK Code Throw UART */
	USART_Transmit(&UART_ONE,(u16)Bootloader_State_ACK);
	/* Debug Information */
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Sending Ack \n",Data_Length);
	#endif
}

/*****************************************************************************************
* Function Name   : Bootloader_Send_Frame
* Description     : Sends a frame over UART, including an acknowledgment, frame size, and 
*                   debug information if enabled.
* Parameters (in) : Data_Length - Size of the frame to be sent.
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
static void Bootloader_Send_Frame(u8 Data_Length)
{	
	/* Acknowlage On Reseved Frame */
	Bootloader_Send_ACK();
	/* Fransmit Frame Size Wanted To Send */
	USART_Transmit(&UART_ONE,(u16)Data_Length);
	/* Debug Information */
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Sending Size Of %d Bytes\n",Data_Length);
	#endif
}

/*****************************************************************************************
* Function Name   : Bootloader_Send_NACK
* Description     : Sends a negative acknowledgment (NACK) code over UART.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
static void Bootloader_Send_NACK(void)
{
	/* Send NACK Code Throw UART */
	USART_Transmit(&UART_ONE,(u16)Bootloader_State_NACK);
}

/*****************************************************************************************
* Function Name   : Bootloader_CRC_Check
* Description     : Performs CRC check on received data.
* Parameters (in) : None
* Parameters (out): None
* Return value    : logic_t - Indicates whether CRC check passed (TRUE) or failed (FALSE).
*****************************************************************************************/
static logic_t Bootloader_CRC_Check(void)
{
	u32 Actual_CRC=ZERO;
	u32 Value=ZERO;
	/* Reset CRC Accumilator Register */
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

/*****************************************************************************************
* Function Name   : Bootloader_Send_Message
* Description     : Sends a formatted message over UART for debugging purposes.
* Parameters (in) : Message - Pointer to the format string containing the message.
*                           - Additional arguments if format string contains format specifiers.
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
static void Bootloader_Send_Message(const u8 *Message,...)
{
	/* Initialize Message Buffer */
	u8 Message_Buffer[Maximum_Buffer_Size]={};
	va_list Arguments;
	va_start(Arguments,Message);
	/* Store Debug Information */
	vsprintf((char *)Message_Buffer,(const char *)Message,Arguments);
	/* Send Debug Information By UART */
	USART_Send_String(&UART_ONE,(const u8 *)Message_Buffer);
	va_end(Arguments);
}

/*****************************************************************************************
* Function Name   : Bootloader_Send_Version
* Description     : Sends the chip ID, software major version, and software minor version 
*                   over UART.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
static void Bootloader_Send_Version(void)
{
	/* Prepare Sending Frame */
	u8 Message[3]={Chip_ID,SW_Major_Version,SW_Minor_Version};
	/* Send ACK And Frame Size */
	Bootloader_Send_Frame(THREE);
	/* Send Frame */
	USART_Send_Array(&UART_ONE,Message,THREE);
	/* Debug Information */
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Done Sending 3 Elements Of Vesrion\n");
	#endif
}

/*****************************************************************************************
* Function Name   : Bootloader_Erase_Flash
* Description     : Erases flash memory pages based on the received start and end page 
*                   numbers, considering only the application region.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
static void Bootloader_Erase_Flash(void)
{
	/* Check For Pages It Its For Application Region */
	if((UART_Buffer[2]>=31)&&(UART_Buffer[3]<128))
	{
		/* Erase Needed Pages */
		if(Flash_State_Ok==Flash_Erase_Pages(UART_Buffer[2],UART_Buffer[3]))
		{
			/* Send ACK Done Erasing */
			Bootloader_Send_ACK();
		}
		/* Send NACK Erasing Failed */
		else{Bootloader_Send_NACK();}
	}
	else
	{
		/* Send NACK Erasing Failed */
		Bootloader_Send_NACK();
	}
}

/*****************************************************************************************
* Function Name   : Bootloader_Send_ID
* Description     : Sends the unique chip ID over UART.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
static void Bootloader_Send_ID(void)
{
	/* Get 3 Byted Unique Chip ID */
	volatile u32 *Variable=&UNIQUE_DEVICE_ID_REGISTER;
	/* Send ACK With Frame Size*/
	Bootloader_Send_Frame(12);
	/* Send Frame */
	USART_Send_Array(&UART_ONE,(u8*)Variable,12);
}

/*****************************************************************************************
* Function Name   : Bootloader_Send_Help
* Description     : Sends a list of available bootloader commands over UART.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
static void Bootloader_Send_Help(void)
{
	/* Prepare Frame */
	u8 Message[]={Bootloader_Command_Send_Help,Bootloader_Command_Send_ID,Bootloader_Command_Send_Version,Bootloader_Command_Erase_Flash,Bootloader_Command_Write_Flash,Bootloader_Command_Address_Jump};
	/* Send ACK With Frame Size*/
	Bootloader_Send_Frame(Total_Services);
	/* Send Frame */
	USART_Send_Array(&UART_ONE,Message,Total_Services);
	/* Debug Information */
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Done Sending Help\n");
	#endif
}

/*****************************************************************************************
* Function Name   : Bootloader_Receive_Payload
* Description     : Receives and validates the payload sent over UART.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Bootloader_State_t - Indicates the status of payload reception and 
*                   validation.
*****************************************************************************************/
static Bootloader_State_t Bootloader_Receive_Payload(void)
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
	/* Debug Information */
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Recevig Payload Done\n");
	#endif
	/* Check For CRC */
	if(TRUE==Bootloader_CRC_Check())
	{
		/* Debug Information */
		#ifdef ENABLE_DEBUG
			Bootloader_Send_Message("CRC Payload Passed\n");
		#endif
	}
	else
	{
		/* Debug Information */
		#ifdef ENABLE_DEBUG
			Bootloader_Send_Message("CRC Failed\n");
		#endif
		/* Update Return State */
		Return=Bootloader_State_Error_CRC;
		/* Report By Not ACK */
		Bootloader_Send_NACK();
	}
	return Return;
}

/*****************************************************************************************
* Function Name   : Bootloader_Write_Flash
* Description     : Writes received payload to flash memory.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Bootloader_State_t - Indicates the status of flash writing process.
*****************************************************************************************/
static Bootloader_State_t Bootloader_Write_Flash(void)
{
	/* Return State */
	Bootloader_State_t Status=Bootloader_State_OK;
	/* Total PayLoad Frames Number */
	u8 Total_Payload_Frames=UART_Buffer[3];
	/* Current Writing Page */
	u8 Writing_Page=UART_Buffer[2];
	/* Counter Of Total Bytes Written */
	u32 Page_Byte_Count=ZERO;
	/* Page Buffer To Write */
	u8 Page_Buffer[1024]={};
	/* Current Received Frame */
	u8 Current_Frame=ZERO;
	/* Erase NEEDED Paged To Be Ready For Writing */
	if(Flash_Erase_Pages(Writing_Page,(((250*Total_Payload_Frames)/1024)+1))==Flash_State_Ok)
	{
		/* Send ACK On Payload Size Frame */
		Bootloader_Send_ACK();
		/* Srart Reveining All Frames */
		while(Current_Frame<Total_Payload_Frames)
		{
			/* Check For Received Frame */
			if(Bootloader_Receive_Payload()==Bootloader_State_OK)
			{
				/* Send ACK If Frame CRC Passed */
				Bootloader_Send_ACK();
				for(u8 Counter=ONE;Counter<UART_Buffer[0]-THREE;Counter++)
				{
					/* Copy From UART Receiving Buffer To Page Buffer */
					Page_Buffer[Page_Byte_Count++]=UART_Buffer[Counter];
					/* Check If Fage Buffer Is Full */
					if((Page_Byte_Count==1024)||((Current_Frame==Total_Payload_Frames-ONE)&&(Counter==UART_Buffer[0]-FOUR)))
					{
						/* Flash Page Buffer To Flash Memory */
						if(Flash_Write_Page(Writing_Page,(u32 *)Page_Buffer)==Flash_State_Ok)
						{
							/* Increment Current Page */
							Writing_Page++;
							/* Reset Page Byte Counter */
							Page_Byte_Count=ZERO;
						}
						/* Update State With Error State */
						else{Status=Bootloader_State_Unsuccessful_Write;}
					}
				}
				/* Increment Receiving Frame */
				Current_Frame++;
			}
			/* Send Error By NACK */
			else{Bootloader_Send_NACK();}
		}
	}
	/* Send Error By NACK */
	else
	{
		Bootloader_Send_NACK();
	}
	/* If It Is Successfull Mark Page Number 32 In Order To Boot To Application Automatically */
	if(Status==Bootloader_State_OK)
	{
		Flash_Write_Data(0x8007FFC,0x0);
	}
	/* Return State */
	return Status;
}

/*****************************************************************************************
* Function Name   : Bootloader_Address_Jump
* Description     : Jumps to the specified address.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
static void Bootloader_Address_Jump(void)
{
	/* Get Jumping Address */
	u32 *Jumping_Address=((u32 *)(*((u32 *)&UART_Buffer[2])));
	/* Check If Valid To Jumb */
	if(((u32)Jumping_Address>Memory_Base)&&((u32)Jumping_Address<(Memory_Base+Memory_Size)))
	{
		/* Send ACK On Payload Size Frame */
		Bootloader_Send_ACK();
		/* Jump On Received Address */
		Bootloader_Start_Application(Jumping_Address);
		/* Debug Information */
		#ifdef ENABLE_DEBUG
			Bootloader_Send_Message("Done Jumping To Address\n");
		#endif
	}
	else
	{
		/* Send Error By NACK */
		Bootloader_Send_NACK();
	}
}

/*****************************************************************************************
* Function Name   : Bootloader_Check_Command
* Description     : Checks and executes the received bootloader command.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Bootloader_State_t - Indicates the status of command execution.
*****************************************************************************************/
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
		case Bootloader_Command_Address_Jump:Bootloader_Address_Jump();break;
		default : Return=Bootloader_State_Wrong_Command;break;
	}
	return Return;
}

/*****************************************************************************************
* Function Name   : Bootloader_Receive_Command
* Description     : Receives and validates the command sent over UART.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Bootloader_State_t - Indicates the status of command reception and 
*                   execution.
*****************************************************************************************/
static Bootloader_State_t Bootloader_Receive_Command(void)
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
	/* Debug Information */
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Recevig Done\n");
	#endif
	if(TRUE==Bootloader_CRC_Check())
	{
		/* Debug Information */
		#ifdef ENABLE_DEBUG
			Bootloader_Send_Message("CRC Passed\n");
		#endif
		Return=Bootloader_Check_Command();
	}
	else
	{
		/* Debug Information */
		#ifdef ENABLE_DEBUG
			Bootloader_Send_Message("CRC Failed\n");
		#endif
		Bootloader_Send_NACK();
	}
	return Return;
}

/*****************************************
------------  APIs Functions  ------------
*****************************************/
/*****************************************************************************************
* Function Name   : Bootloader_Jump
* Description     : Initiates a jump from the current application to the bootloader 
*                   application. This function erases a flag to ensure the system stays 
*                   in bootloader mode during the next boot, and then jumps to the 
*                   bootloader application.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Bootloader_Jump(void)
{
	/* Erase Flag To Stay In Bootloader Application */
	Flash_Erase_Pages(31,1);
	/* Perform Software Reset */
	SCB.AIRCR.Register=SOFTWARE_RESET_KEY;
	/* Dummy read to ensure completion before the next instruction */
	(void)SCB.AIRCR.Register;
	/* Stuck If Failed To Reset */
	while(1);
}

/*****************************************************************************************
* Function Name   : Bootloader_Start
* Description     : Starts the bootloader application. It checks for a flag in the flash 
*                   memory. If the flag is set, it waits to receive a command from the 
*                   host. Otherwise, it starts the flashed application.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Bootloader_Start(void)
{
	/* Check For Flag */
	if((Application_Base-1)[0]==0xffffffff)
	{
		/* Wait Till Receiving Command From Host */
		Bootloader_Receive_Command();
	}
	else
	{
		/* Start Flashed Application */
		Bootloader_Start_Application(Application_Base);
	}
}

/*****************************************************************************************
* Function Name   : Bootloader_Initialize
* Description     : Initializes necessary modules for the bootloader operation.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
*****************************************************************************************/
void Bootloader_Initialize(void)
{
	/* Initialize UART For Bootloader */
	USART_Initialization(&UART_ONE);
	/* Initialize CRC Modlue */
	CRC_Initialization();
}

/********************************************************************
 *  END OF FILE:  Bootloader.c
********************************************************************/