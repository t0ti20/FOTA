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
#include "Bootloader_Interface.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
static u8 Chip_ID=Chip_ID_Number;
static u8 UART_Buffer[Maximum_Buffer_Size];
static u8 SW_Major_Version=Basic_SW_Major_Version;
static u8 SW_Minor_Version=Basic_SW_Minor_Version;
static USART_Config_t UART_ONE={NULL,USART_1,USART_No_Parity,USART_Rx_Tx,Disable,USART_Eight_Bits,USART_One_Stop,USART_115200,USART_Disable_Interrupt};
/*****************************************
-----------  Static Functions  -----------
*****************************************/
static void Bootloader_Start_Application(volatile u32 *Application_Address)
{		
	void (*Application_Reset)(void);
	#ifdef ENABLE_DEBUG
	Bootloader_Send_Message("Starting User Application\n",Data_Length);
	#endif
	/* Application Stack Pointer */
	SET_MSP(Application_Address[0]);
	/* Application Reset Handler */
	Application_Reset=(void *)Application_Address[1];
	/* Deinitialization Of All Modules */
	/* Jump To Application */
	Application_Reset();
}
static void Bootloader_Send_ACK()
{		
	#ifdef ENABLE_DEBUG
	Bootloader_Send_Message("Sending Ack \n",Data_Length);
	#endif
	USART_Transmit(&UART_ONE,(u16)Bootloader_State_ACK);
}
static void Bootloader_Send_Frame(u8 Data_Length)
{	
	Bootloader_Send_ACK();
	#ifdef ENABLE_DEBUG
	Bootloader_Send_Message("Sending Size Of %d Bytes\n",Data_Length);
	#endif
	USART_Transmit(&UART_ONE,(u16)Data_Length);
}

/****************************************/
static void Bootloader_Send_NACK(void)
{
	USART_Transmit(&UART_ONE,(u16)Bootloader_State_NACK);
}
/****************************************/
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
/****************************************/
static void Bootloader_Send_Version(void)
{
	u8 Message[3]={Chip_ID,SW_Major_Version,SW_Minor_Version};
	Bootloader_Send_Frame(THREE);
	#ifdef ENABLE_DEBUG
	Bootloader_Send_Message("Done Sending 3 Elements Of Vesrion\n");
	#endif
	USART_Send_Array(&UART_ONE,Message,THREE);
}
/****************************************/
static void Bootloader_Erase_Flash(void)
{
	if((UART_Buffer[2]>=32)&&(UART_Buffer[3]<128))
	{
		if(Flash_State_Ok==Flash_Erase_Pages(UART_Buffer[2],UART_Buffer[3]))
		{
			Bootloader_Send_ACK();
		}
		else{Bootloader_Send_NACK();}
	}
	else
	{
		Bootloader_Send_NACK();
	}
}
/****************************************/
static void Bootloader_Send_ID(void)
{
	volatile u32 *Variable=&UNIQUE_DEVICE_ID_REGISTER;
	Bootloader_Send_Frame(12);
	USART_Send_Array(&UART_ONE,(u8*)Variable,12);
}
/****************************************/
static void Bootloader_Send_Help(void)
{
	u8 Message[]={Bootloader_Command_Send_Help,Bootloader_Command_Send_ID,Bootloader_Command_Send_Version,Bootloader_Command_Erase_Flash,Bootloader_Command_Write_Flash,Bootloader_Command_Address_Jump};
	Bootloader_Send_Frame(Total_Services);
	USART_Send_Array(&UART_ONE,Message,Total_Services);
	#ifdef ENABLE_DEBUG
	Bootloader_Send_Message("Done Sending Help\n");
	#endif
}
/****************************************/
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
	#ifdef ENABLE_DEBUG
		Bootloader_Send_Message("Recevig Payload Done\n");
	#endif
	if(TRUE==Bootloader_CRC_Check())
	{
		#ifdef ENABLE_DEBUG
			Bootloader_Send_Message("CRC Payload Passed\n");
		#endif
		Return=Bootloader_State_OK;
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
static Bootloader_State_t Bootloader_Write_Flash(void)
{
	u8 Page_Buffer[1024]={};
	Bootloader_State_t Status=Bootloader_State_OK;
	u8 Total_Payload_Frames=UART_Buffer[3];
	u8 Writing_Page=UART_Buffer[2];
	u8 Current_Frame=ZERO;
	u16 Page_Byte_Count=ZERO;
	if(Flash_Erase_Pages(Writing_Page,(((250*Total_Payload_Frames)/1024)+1))==Flash_State_Ok)
	{
		Bootloader_Send_ACK();
		while(Current_Frame<Total_Payload_Frames)
		{
			if(Bootloader_Receive_Payload()==Bootloader_State_OK)
			{
				Bootloader_Send_ACK();
				for(u8 Counter=ONE;Counter<UART_Buffer[0]-THREE;Counter++)
				{
					Page_Buffer[Page_Byte_Count++]=UART_Buffer[Counter];
					if((Page_Byte_Count==1024)||((Current_Frame==Total_Payload_Frames-ONE)&&(Counter==UART_Buffer[0]-FOUR)))
					{
						if(Flash_Write_Page(Writing_Page,(u32 *)Page_Buffer)==Flash_State_Ok)
						{
							Writing_Page++;
							Page_Byte_Count=ZERO;
						}
						else{Status=Bootloader_State_Unsuccessful_Write;}
					}
				}
				Current_Frame++;
			}
			else{Bootloader_Send_NACK();}
		}
	}
	else
	{
		Bootloader_Send_NACK();
	}
	if(Status==Bootloader_State_OK)
	{
		Flash_Write_Data(0x8007FFC,0x0);
	}
	return Status;
}
/****************************************/
static void Bootloader_Address_Jump(void)
{
	/* Get Jumping Address */
	u32 *Jumping_Address=((u32 *)(*((u32 *)&UART_Buffer[2])));
	/* Check If Valid To Jumb */
	if(((u32)Jumping_Address>Memory_Base)&&((u32)Jumping_Address<(Memory_Base+Memory_Size)))
	{
		Bootloader_Send_ACK();
		Bootloader_Start_Application(Jumping_Address);
		#ifdef ENABLE_DEBUG
			Bootloader_Send_Message("Done Jumping To Address\n");
		#endif
	}
	else
	{
		Bootloader_Send_NACK();
	}
}
/****************************************/
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
/****************************************/
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
/*****************************************
-----------  Static Functions  -----------
*****************************************/
void Bootloader_Jump(void)
{
	Flash_Erase_Pages(31,1);
	Bootloader_Start_Application(Bootloader_Base);
}
void Bootloader_Start(void)
{
	if((Application_Base-1)[0]==0xffffffff)
	{
		Bootloader_Receive_Command();
	}
	else
	{
		Bootloader_Start_Application(Application_Base);
	}
}

/****************************************/
void Bootloader_Send_Message(const u8 *Message,...)
{
	u8 Message_Buffer[Maximum_Buffer_Size]={};
	va_list Arguments;
	va_start(Arguments,Message);
	vsprintf((char *)Message_Buffer,(const char *)Message,Arguments);
	USART_Send_String(&UART_ONE,(const u8 *)Message_Buffer);
	va_end(Arguments);
}
/****************************************/
void Bootloader_Initialize(void)
{
	USART_Initialization(&UART_ONE);
	CRC_Initialization();
}
/********************************************************************
 *  END OF FILE:  Bootloader.c
********************************************************************/