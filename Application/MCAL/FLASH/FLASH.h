/*********************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Owner:  Khaled El-Sayed @t0ti20
 *  File:  FLASH.h
 *  Module:  MCAL/FLASH
 *  Description:  Flash Driver Interface File
*********************************************************************/
#ifndef _FLASH_H_
#define _FLASH_H_
/********************************************************************/
/*****************************************
---------    Configure Macros    ---------
*****************************************/
/*-------------- Unlock Key ------------*/
#define KEY1                            (0x45670123UL)
#define KEY2                            (0xCDEF89ABUL)
/*----------- Pages Addresses ----------*/
#define Flash_Start                     (0x08000000UL)
#define Flash_Page_Size                 (0x00000400UL)
#define Flash_Maximum_Pages             (128U)
/*****************************************
--------    Type  Definitions    ---------
*****************************************/
/*------------- Flash State ------------*/
typedef enum FLASH_State_t
{
     Flash_State_Ok              =(0),
     Flash_State_Error           =(1)
}FLASH_State_t;
/*****************************************
---  Application Programming Interface  --
*****************************************/
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
FLASH_State_t Flash_Erase_Pages(u8 Start_Page,u8 Pages_Number);

/*****************************************************************************************
* Service Name           : Flash_Write_Pages
* Syntax                 : FLASH_State_t Flash_Write_Pages(u8 Start_Page, u16 Data[], u32 Data_Size)
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
FLASH_State_t Flash_Write_Pages(u8 Start_Page,u16 Data[],u32 Data_Size);

/********************************************************************
 *  END OF FILE:  FLASH.h
********************************************************************/
#endif
