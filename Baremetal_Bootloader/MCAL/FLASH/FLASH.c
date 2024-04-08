/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  FLASH.c
 *  Module:  MCAL/FLASH
 *  Description:  FLASH Program File For STM32F103C6
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "STM32F103.h"
#include "FLASH.h"
/*****************************************
----------    GLOBAL DATA     ------------
*****************************************/
/*****************************************************************************************
* Service Name           : FLASH_Lock
* Syntax                 : static void FLASH_Lock(void)
* Sync/Async             : Synchronous
* Reentrancy             : Reentrant
* Parameters (in)        : None
* Parameters (in-out)    : None
* Parameters (out)       : None
* Return value           : None
* Description            : Locks the FLASH for safety.
* Available via          : FLASH.c
*****************************************************************************************/
static void FLASH_Lock(void)
{
     FLASH.CR.Bits.Bit_7=Enable;
}

/*****************************************************************************************
* Service Name           : FLASH_Unlock
* Syntax                 : static FLASH_State_t FLASH_Unlock(void)
* Sync/Async             : Synchronous
* Reentrancy             : Reentrant
* Parameters (in)        : None
* Parameters (in-out)    : None
* Parameters (out)       : None
* Return value           : FLASH_State_t
* Description            : Unlocks the FLASH to perform operations.
* Available via          : FLASH.c
*****************************************************************************************/
static FLASH_State_t FLASH_Unlock(void)
{
     FLASH_State_t Status=Flash_State_Ok;
     /*Unlock Flash*/
     FLASH.KEYR.Register=KEY1;
     FLASH.KEYR.Register=KEY2;
     /*Check If Its Unloked*/
     if(FLASH.CR.Bits.Bit_7)
     {
          Status=Flash_State_Error;
     }
     else{}
     return Status;
}

/*****************************************************************************************
* Service Name           : Flash_Erase_Page
* Syntax                 : static FLASH_State_t Flash_Erase_Page(u32 pageAddress)
* Sync/Async             : Synchronous
* Reentrancy             : Reentrant
* Parameters (in)        : pageAddress -> The address of the start of the page to be erased.
* Parameters (in-out)    : None
* Parameters (out)       : None
* Return value           : FLASH_State_t -> Status of the erase operation.
* Description            : Service to erase a specific page in the FLASH memory. It ensures that
*                          no flash operation is ongoing, sets the target address, initiates the 
*                          erase, and waits for its completion or detection of any error.
* Available via          : FLASH.c
*****************************************************************************************/
static FLASH_State_t Flash_Erase_Page(u32 pageAddress)
{
     FLASH_State_t Status=Flash_State_Ok;
     /*Ensure no flash operation is ongoing*/
     while (FLASH.SR.Bits.Bit_0);
     /*Set the PER (Page Erase) bit in the Flash control register*/
     FLASH.CR.Bits.Bit_1=Enable;
     /*Set the page address*/
     FLASH.AR.Register=pageAddress;
     /*Start the erase operation*/
     FLASH.CR.Bits.Bit_6=Enable;
     /*Wait until the erase operation is completed (or an error occurs)*/
     while (FLASH.SR.Bits.Bit_0);
     // Check for errors (e.g., write protection error)
     if (FLASH.SR.Bits.Bit_2||FLASH.SR.Bits.Bit_4)
     {
          /*Clear the error flags*/
          FLASH.SR.Bits.Bit_2=Enable;
          FLASH.SR.Bits.Bit_4=Enable;
          Status=Flash_State_Error;
     }
     else
     {
          /*Clear the PER bit*/
          FLASH.CR.Bits.Bit_1=Disable;
     }
     return Status;
}

/*****************************************************************************************
* Service Name           : FLASH_Halfword_Write
* Syntax                 : static FLASH_State_t FLASH_Halfword_Write(u32 Address, u16 Data)
* Sync/Async             : Synchronous
* Reentrancy             : Reentrant
* Parameters (in)        : Address -> The flash address where the halfword data should be written.
*                          Data    -> The halfword data to be written to the specified address.
* Parameters (in-out)    : None
* Parameters (out)       : None
* Return value           : FLASH_State_t -> Status of the halfword write operation.
* Description            : Service to write a halfword (16 bits) to a specific address in the FLASH memory.
*                          The function first enables halfword programming, writes the data, waits for
*                          the write operation to complete, and then checks if the write was successful.
* Available via          : FLASH.c
*****************************************************************************************/
static FLASH_State_t FLASH_Halfword_Write(u32 Address, u16 Data)
{
     FLASH_State_t Status=Flash_State_Ok;
     /* Enable the half-word program option (set Bit_0 in FLASH_CR register) */
     FLASH.CR.Bits.Bit_0=1;
     /* Write the half-word data to the desired address */
     (*(volatile u16 *)Address)= Data;
     /* Wait until the flash is ready (wait until BSY bit in FLASH_SR register is cleared) */
     while (FLASH.SR.Bits.Bit_0){}
     /* Check for error conditions */
     if ((*(volatile u16 *)Address)!=Data)
     {
          Status=Flash_State_Error;
     }
     /* Disable the half-word program option (clear Bit_0 in FLASH_CR register) */
     FLASH.CR.Bits.Bit_0 = 0;
     return Status;
}
/*****************************************************************************************
* Service Name           : Flash_Erase_Pages
* Syntax                 : FLASH_State_t Flash_Erase_Pages(u8 Start_Page, u8 Pages_Number)
* Sync/Async             : Synchronous
* Reentrancy             : Reentrant
* Parameters (in)        : Start_Page   -> The starting page number to begin the erase.
*                          Pages_Number -> The total number of pages to be erased.
* Parameters (in-out)    : None
* Parameters (out)       : None
* Return value           : FLASH_State_t -> Status of the pages erase operation.
* Description            : Service to erase a specific range of pages in the FLASH memory.
*                          The function calculates the start address using the Start_Page number, 
*                          then iteratively erases each page until all specified pages are erased
*                          or an error occurs.
* Available via          : FLASH.c
*****************************************************************************************/
FLASH_State_t Flash_Erase_Pages(u8 Start_Page,u8 Pages_Number)
{
     FLASH_State_t Status=Flash_State_Ok;
     u32 Page_Counter=(Flash_Start+(Start_Page*Flash_Page_Size));
     Status=FLASH_Unlock();
     for(;(Pages_Number>ZERO)&&(Flash_State_Ok==Status);Pages_Number--)
     {
          Status=Flash_Erase_Page(Page_Counter);
          Page_Counter+=Flash_Page_Size;
     }
     FLASH_Lock();
     return Status;
}
/*****************************************************************************************
* Service Name           : Flash_Write_Pages
* Syntax                 : FLASH_State_t Flash_Write_Pages(u8 Start_Page, u32 Data[], u32 Data_Size)
* Sync/Async             : Synchronous
* Reentrancy             : Reentrant
* Parameters (in)        : Start_Page  -> The starting page number to begin the write operation.
*                          Data        -> The array of data to be written to the FLASH memory.
*                          Data_Size   -> The size of the data array, indicating how many 32-bit 
*                                         values should be written.
* Parameters (in-out)    : None
* Parameters (out)       : None
* Return value           : FLASH_State_t -> Status of the pages write operation.
* Description            : Service to write a specific range of data to the FLASH memory.
*                          It first calculates the start address using the Start_Page number, then
*                          erases the necessary pages to accommodate the data. It iteratively writes
*                          each 32-bit value from the data array to the FLASH memory, handling each
*                          value as two halfwords.
* Available via          : FLASH.c
*****************************************************************************************/
FLASH_State_t Flash_Write_Page(u8 Page_Number,u32 Data[]) 
{
     u16 Counter = ZERO;
     FLASH_State_t Status = Flash_State_Ok;
     u32 Address_Pointer = (Flash_Start + (Page_Number * Flash_Page_Size));
     /* Unlock flash for write operations */
     Status = FLASH_Unlock();
     /* Loop through the data and write to flash memory */
     for (Counter=ZERO;Counter<(Flash_Page_Size/FOUR)&&(Status==Flash_State_Ok);Counter++,Address_Pointer+=FOUR)
     {
          /* Write the lower 16 bits of Data[Counter] */
          Status = FLASH_Halfword_Write(Address_Pointer,(u16)(Data[Counter] & 0xFFFF));
          /* Check if the lower 16 bits were written successfully before writing upper 16 bits */
          if (Status == Flash_State_Ok)
          {
               /* Write the upper 16 bits of Data[Counter] */
               Status = FLASH_Halfword_Write(Address_Pointer+2,(u16)(Data[Counter]>>16));
          }
          else {break;}
     }
     /* Lock flash after write operations */
     FLASH_Lock();
     return Status;
}
FLASH_State_t Flash_Write_Data(u32 Address,u32 Data) 
{
     FLASH_State_t Status = Flash_State_Ok;
     /* Unlock flash for write operations */
     if (FLASH_Unlock()==Flash_State_Ok)
     {
          /* Check if the lower 16 bits were written successfully before writing upper 16 bits */
          if (FLASH_Halfword_Write(Address,(u16)(Data&0xFFFF))==Flash_State_Ok)
          {
               /* Write the upper 16 bits of Data[Counter] */
               Status = FLASH_Halfword_Write(Address+2,(u16)(Data>>16));
          }
     }
     /* Lock flash after write operations */
     FLASH_Lock();
     return Status;
}
/********************************************************************
 *  END OF FILE:  FLASH.c
********************************************************************/
