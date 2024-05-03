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
#include "Macros.h"
#include "USART_Interface.h"
#include "FLASH.h"
#include "CRC.h"

/*****************************************
-------    Functions Prototypes   --------
*****************************************/
static void Start_Bootloader_Interrupt(u16 *Data);
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/* USART Configure */
static USART_Config_t Bootloader_UART={Start_Bootloader_Interrupt,USART_1,USART_No_Parity,USART_Rx_Tx,Disable,USART_Eight_Bits,USART_One_Stop,USART_115200,USART_Rx_Complete_Interrupt};
/* Switch To Bootloader Flag */
u8 Open_Bootloader=FALSE;
/* Version Control */
const u32 volatile Version_Controller __attribute__((section(".VERSION"))) = SET_VERSION(Default_Chip_ID_Number,Default_SW_Major_Version,Default_SW_Minor_Version);
const u32 volatile Aplication_Valid __attribute__((section(".APPLICATION_CRC")))=0xffffffff;
/* UART Buffer */
static u8 UART_Buffer[Maximum_Buffer_Size];
/*****************************************
-----------  Static Functions  -----------
*****************************************/
/****************************************************************************************************
* Function Name   : Start_Bootloader_Interrupt
* Description     : Static function to start bootloader interrupt.
* Parameters (in) : Data - Pointer to u16 data.
* Parameters (out): None
* Return value    : None
* Notes           : - This function configures USART to run without interrupt.
*****************************************************************************************************/
static void Start_Bootloader_Interrupt(u16 *Data)
{
	/* Configure USART To Run Without Interrupt */
	Open_Bootloader=TRUE;
	Bootloader_UART.Call_Back_Function=NULL;
	Bootloader_UART.USART_Interrupt=USART_Disable_Interrupt;
}

/****************************************************************************************************
* Function Name   : Reset_MCU
* Description     : Static function to perform software reset of MCU.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function performs a software reset of the MCU.
*                   - It writes the SOFTWARE_RESET_KEY to SCB AIRCR register.
*                   - It ensures completion before proceeding to the next instruction.
*                   - It enters an infinite loop if the reset fails.
*****************************************************************************************************/
static void Reset_MCU(void)
{
	/* Perform Software Reset */
	SCB.AIRCR.Register=SOFTWARE_RESET_KEY;
	/* Dummy read to ensure completion before the next instruction */
	(void)SCB.AIRCR.Register;
	/* Stuck If Failed To Reset */
	while(1);
}

/****************************************************************************************************
* Function Name   : Bootloader_Start_Application
* Description     : Static function to start the user application.
* Parameters (in) : Application_Address - Pointer to the application address.
* Parameters (out): None
* Return value    : None
* Notes           : - This function sets the stack pointer and reset handler for the user application.
*                   - It deinitializes all modules (TODO).
*                   - It provides debug information if ENABLE_DEBUG is defined.
*                   - It jumps to the application reset handler to start the user application.
*****************************************************************************************************/
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
	USART_Transmit(&Bootloader_UART,(u16)Bootloader_State_ACK);
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
	USART_Transmit(&Bootloader_UART,(u16)Data_Length);
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
	USART_Transmit(&Bootloader_UART,(u16)Bootloader_State_NACK);
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
#ifdef ENABLE_DEBUG
static void Bootloader_Send_Message(const u8 *Message,...)
{
	/* Initialize Message Buffer */
	u8 Message_Buffer[Maximum_Buffer_Size]={};
	va_list Arguments;
	va_start(Arguments,Message);
	/* Store Debug Information */
	vsprintf((char *)Message_Buffer,(const char *)Message,Arguments);
	/* Send Debug Information By UART */
	USART_Send_String(&Bootloader_UART,(const u8 *)Message_Buffer);
	va_end(Arguments);
}
#endif
/****************************************************************************************************
* Function Name   : Bootloader_Send_Version
* Description     : Static function to send the version information.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function prepares and sends the version information frame.
*                   - It sends ACK and frame size before sending the frame.
*                   - It provides debug information if ENABLE_DEBUG is defined.
*****************************************************************************************************/
static void Bootloader_Send_Version(void)
{
	u8 const *Version=(const u8 *)Version_Location;
	/* Prepare Sending Frame */
	u8 Message[3]={Version[1],Version[2],Version[3]};
	/* Send ACK And Frame Size */
	Bootloader_Send_Frame(THREE);
	/* Send Frame */
	USART_Send_Array(&Bootloader_UART,Message,THREE);
	/* Debug Information */
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Done Sending 3 Elements Of Vesrion\n");
	#endif
}

/****************************************************************************************************
* Function Name   : Bootloader_Erase_Flash
* Description     : Static function to erase flash memory.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function checks for pages in the application region to erase.
*                   - If the erase operation is successful, it sends an ACK.
*                   - If the erase operation fails, it sends a NACK.
*****************************************************************************************************/
static void Bootloader_Erase_Flash(void) __attribute__((section(".FLASH_OPERATIONS")));
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
	USART_Send_Array(&Bootloader_UART,(u8*)Variable,12);
}

/****************************************************************************************************
* Function Name   : Bootloader_Send_Help
* Description     : Static function to send help information.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function prepares and sends the help information frame.
*                   - It sends ACK with frame size before sending the frame.
*                   - It provides debug information if ENABLE_DEBUG is defined.
*****************************************************************************************************/
static void Bootloader_Send_Help(void)
{
	/* Prepare Frame */
	u8 Message[]={Bootloader_Command_Send_Help,Bootloader_Command_Send_ID,Bootloader_Command_Send_Version,Bootloader_Command_Erase_Flash,Bootloader_Command_Flash_Application,Bootloader_Command_Address_Jump};
	/* Send ACK With Frame Size*/
	Bootloader_Send_Frame(Total_Services);
	/* Send Frame */
	USART_Send_Array(&Bootloader_UART,Message,Total_Services);
	/* Debug Information */
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Done Sending Help\n");
	#endif
}

/****************************************************************************************************
* Function Name   : Bootloader_Receive_Payload
* Description     : Static function to receive payload from the bootloader.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Bootloader_State_t - State of the bootloader after receiving payload.
* Notes           : - This function receives the frame size and the entire frame.
*                   - It checks the CRC of the payload and updates the return state accordingly.
*                   - It provides debug information if ENABLE_DEBUG is defined.
*****************************************************************************************************/
static Bootloader_State_t Bootloader_Receive_Payload(void)
{
	Bootloader_State_t Return=Bootloader_State_OK;
	u8 Frame_Length=ZERO;
	/* Receive Frame Size */
	UART_Buffer[ZERO]=(u8)USART_Receive(&Bootloader_UART);
	/* Receive Whole Frame */
	for(Frame_Length=ONE;Frame_Length<=UART_Buffer[ZERO];Frame_Length++)
	{
		UART_Buffer[Frame_Length]=(u8)USART_Receive(&Bootloader_UART);
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

/****************************************************************************************************
* Function Name   : Bootloader_Write_Flash
* Description     : Static function to write flash memory with received payload.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Bootloader_State_t - State of the bootloader after writing flash.
* Notes           : - This function erases needed pages to be ready for writing.
*                   - It receives all frames of the payload, writes them to flash memory, and
*                     sends ACK or NACK based on the CRC of each frame.
*                   - It updates the state based on the success or failure of writing.
*                   - If writing is successful, it marks page number 32 to boot to the application
*                     automatically.
*****************************************************************************************************/
static Bootloader_State_t Bootloader_Write_Flash(void) __attribute__((section(".FLASH_OPERATIONS")));
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
							/* Flush Buffer */
							memset(Page_Buffer,0xff, sizeof(Page_Buffer));
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
	/* Return State */
	return Status;
}
/****************************************************************************************************
* Function Name   : Bootloader_Address_Jump
* Description     : Static function to jump to a specified address.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function receives the jumping address from the UART buffer.
*                   - It checks if the address is valid for jumping.
*                   - If valid, it sends ACK and jumps to the received address.
*****************************************************************************************************/
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
/****************************************************************************************************
* Function Name   : Bootloader_Say_Bye
* Description     : Static function to send a bye message and reset the MCU.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function sends an ACK and frame size.
*                   - It resets the MCU after sending the message.
*****************************************************************************************************/
static void Bootloader_Say_Bye(void)
{
	/* Send ACK And Frame Size */
	Bootloader_Send_Frame(ZERO);
	/* Debug Information */
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Reset MCU\n");
	#endif
	Reset_MCU();
}
/****************************************************************************************************
* Function Name   : Bootloader_Say_Hi
* Description     : Static function to send a hi message.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function sends an ACK and frame size.
*****************************************************************************************************/
static void Bootloader_Say_Hi(void)
{
	/* Send ACK And Frame Size */
	Bootloader_Send_Frame(ZERO);
	/* Debug Information */
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Done Sending 3 Elements Of Vesrion\n");
	#endif
}
/****************************************************************************************************
* Function Name   : Bootloader_Send_Data
* Description     : Static function to send data.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function reads desired address and data from UART buffer and checks if it is in the application region.
*                   - If yes, it writes the data to flash memory at the desired address and sends an ACK. Otherwise, it sends a NACK.
*****************************************************************************************************/
static void Bootloader_Send_Data(void) __attribute__((section(".FLASH_OPERATIONS")));
static void Bootloader_Send_Data(void)
{
	u32 Desired_Address=(*(u32*)(UART_Buffer+2));
	u32 Desired_Data=(*(u32*)(UART_Buffer+6));
	/* Check For Pages It Its For Application Region */
	if((Desired_Address>=Memory_Base)&&(Desired_Address<(Memory_Size+Memory_Base)))
	{
		/* Erase Needed Pages */
		if(Flash_State_Ok==Flash_Write_Data(Desired_Address,Desired_Data))
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

/****************************************************************************************************
* Function Name   : Bootloader_Check_Command
* Description     : Static function to check and execute bootloader command.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Bootloader_State_t - State of the bootloader operation.
* Notes           : - This function checks the received bootloader command and executes the corresponding action.
*                   - It returns the state of the bootloader operation.
*****************************************************************************************************/
static Bootloader_State_t Bootloader_Check_Command(void)
{
	Bootloader_State_t Return=Bootloader_State_OK;
	switch ((Bootloader_Command_t)UART_Buffer[1])
	{
		case Bootloader_Command_Send_Version:Bootloader_Send_Version();break;
		case Bootloader_Command_Send_Help:Bootloader_Send_Help();break;
		case Bootloader_Command_Send_ID:Bootloader_Send_ID();break;
		case Bootloader_Command_Erase_Flash:Bootloader_Erase_Flash();break;
		case Bootloader_Command_Flash_Application:Bootloader_Write_Flash();break;
		case Bootloader_Command_Address_Jump:Bootloader_Address_Jump();break;
		case Bootloader_Command_Say_Hi:Bootloader_Say_Hi();break;
		case Bootloader_Command_Say_Bye:Bootloader_Say_Bye();break;
		case Bootloader_Command_Send_Data:Bootloader_Send_Data();break;
		default : Return=Bootloader_State_Wrong_Command;break;
	}
	return Return;
}

/****************************************************************************************************
* Function Name   : Bootloader_Receive_Command
* Description     : Static function to receive and process the bootloader command.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Bootloader_State_t - State of the bootloader operation.
* Notes           : - This function receives the bootloader command, checks its CRC, and processes it.
*                   - It returns the state of the bootloader operation.
*****************************************************************************************************/
static Bootloader_State_t Bootloader_Receive_Command(void)
{
	Bootloader_State_t Return=Bootloader_State_OK;
	u8 Frame_Length=ZERO;
	/* Receive Frame Size */
	UART_Buffer[ZERO]=(u8)USART_Receive(&Bootloader_UART);
	/* Receive Whole Frame */
	for(Frame_Length=ONE;Frame_Length<=UART_Buffer[ZERO];Frame_Length++)
	{
		UART_Buffer[Frame_Length]=(u8)USART_Receive(&Bootloader_UART);
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
bool Validate (void)
{
	bool Is_Valid=false;
	u32 volatile *Iterator=NULL;
	u32 CRC_Result=ZERO;
	CRC_Reset();
	for(Iterator=Application_Base;Iterator<End_Memory;++Iterator)
	{
		CRC_Result=CRC_Accumulate(*Iterator);
	}
	if(CRC_Result==Valid_CRC){Is_Valid=true;}
	return Is_Valid;
}
/****************************************************************************************************
* Function Name   : Bootloader_Start
* Description     : Function to start the bootloader operation.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function is the entry point for the bootloader operation.
*                   - It continuously checks if the bootloader should be opened, the application should be started, or the bootloader should be interrupted.
*****************************************************************************************************/
void Bootloader_Start(void)
{
	while(1)
	{
		if(Open_Bootloader)
		{
			Bootloader_Initialize();
			while(1){Bootloader_Receive_Command();}
		}
		else if(Validate())
		{
			USART_Reset(&Bootloader_UART);
			Bootloader_Start_Application(Application_Base);
		}
		else
		{
			u16 Dummy_Variable;
			Start_Bootloader_Interrupt(&Dummy_Variable);
		}
	}
}
/****************************************************************************************************
* Function Name   : Bootloader_Initialize
* Description     : Function to initialize the bootloader.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function initializes the UART and CRC modules used by the bootloader.
*****************************************************************************************************/
void Bootloader_Initialize(void)
{
	USART_Reset(&Bootloader_UART);
	USART_Initialization(&Bootloader_UART);
	/* Initialize CRC Modlue */
	CRC_Initialization();
}

/********************************************************************
 *  END OF FILE:  Bootloader_Program.c
********************************************************************/