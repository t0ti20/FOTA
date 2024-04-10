/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
*  Author: Khaled El-Sayed @t0ti20
*  File: Bootloader_Host.h
*  Date: March 28, 2024
*  Description: Bootloader Host For STM32
*  Namespace : Bootloader
*  (C) 2024 "@t0ti20". All rights reserved.
*******************************************************************/
#ifndef _BOOTLOADER_INTERFACE_H_
#define _BOOTLOADER_INTERFACE_H_
/******************************************************************/
/*****************************************
------------    Includes     -------------
*****************************************/
#include <vector>
#include <thread>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/asio.hpp>
/*****************************************
---------    Configurations     ----------
*****************************************/
//#define ENABLE_DEBUG                            (4)
constexpr unsigned char Bootloader_State_ACK    {1};
constexpr unsigned char Bootloader_State_NACK   {2};
constexpr unsigned int Sending_Delay_MS         {100};
constexpr unsigned int Get_Update_Time_Seconds  {5};
constexpr unsigned int Application_Location     {32};
constexpr unsigned int Booting_Flag             {0x8007FFC};
/*****************************************
-----------    Bootloader     ------------
*****************************************/
namespace Bootloader
{
/*****************************************
-----------    GPIO_Manage     -----------
*****************************************/
class GPIO_Manage
{
/*************** Methods ****************/
protected:
/****************************************************************************************************
* Constructor Name: GPIO_Manage
* Class           : GPIO_Manage
* Namespace       : Bootloader
* Description     : Initializes GPIO management by exporting a specified pin and setting its direction.
* Parameters (in) : GPIO_Manage_Pin - The GPIO pin number to manage.
* Parameters (out): None
* Return value    : None
* Notes           : - This constructor opens the necessary files for exporting GPIO pins and setting pin direction.
*                   - It exports the specified pin, sets its direction as output, and prints error messages if any.
*****************************************************************************************************/
GPIO_Manage(const std::string &GPIO_Manage_Pin);
/****************************************************************************************************
* Destructor Name : ~GPIO_Manage
* Class           : GPIO_Manage
* Namespace       : Bootloader
* Description     : Destructs GPIO management by unexporting the GPIO pin.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This destructor opens the necessary file for unexporting GPIO pins.
*                   - It unexports the specified pin and prints an error message if any.
*****************************************************************************************************/
~GPIO_Manage();
/****************************************************************************************************
* Function Name   : Halt_MCU
* Class           : GPIO_Manage
* Namespace       : Bootloader
* Description     : Halts the microcontroller by triggering an interrupt to run the bootloader application.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function toggles a pin to generate an interrupt, triggering the bootloader application.
*                   - It first sets the pin, then prints a message, and finally clears the pin.
*****************************************************************************************************/
void Halt_MCU(void);
private:
/****************************************************************************************************
* Function Name   : Set_Pin
* Class           : GPIO_Manage
* Namespace       : Bootloader
* Description     : Sets the GPIO pin to a high state.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function opens the file for setting the pin value to high.
*                   - It sets the pin value to "1" to set it high and prints an error message if any.
*****************************************************************************************************/
void Set_Pin(void);
/****************************************************************************************************
* Function Name   : Clear_Pin
* Class           : GPIO_Manage
* Namespace       : Bootloader
* Description     : Clears the GPIO pin by setting it to a low state.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function opens the file for setting the pin value to low.
*                   - It sets the pin value to "0" to clear it and prints an error message if any.
*****************************************************************************************************/
void Clear_Pin(void);
/*************** Variables **************/
private:
std::string Pin_Number{};
std::ofstream Pin_Handlar;
};
/*****************************************
------------    CRC_Manage     -----------
*****************************************/
class CRC_Manage
{
/*************** Methods ****************/
protected:
/****************************************************************************************************
* Function Name   : Append_CRC
* Class           : CRC_Manage
* Namespace       : Bootloader
* Description     : Appends CRC (Cyclic Redundancy Check) to the given data vector.
* Parameters (in) : Data - Reference to a vector of unsigned characters representing the data.
* Parameters (out): Data - Updated data vector with CRC appended.
* Return value    : None
* Notes           : - This function adds padding bits to the data until its size is a multiple of 4.
*                   - It calculates CRC for the data using CRC_Calculate function.
*                   - After calculation, it removes the padding bits and appends the CRC to the data.
*****************************************************************************************************/
void Append_CRC(std::vector<unsigned char> &Data);

private:
/****************************************************************************************************
* Function Name   : CRC_Calculate
* Class           : CRC_Manage
* Namespace       : Bootloader
* Description     : Calculates CRC (Cyclic Redundancy Check) for the given data.
* Parameters (in) : Data - Reference to a vector of unsigned characters representing the data.
* Parameters (out): None
* Return value    : Unsigned integer representing the calculated CRC value.
* Notes           : - This function uses CRC32 algorithm to calculate CRC.
*                   - It initializes CRC with CRC32_INIT value, and then iterates through each byte of the data,
*                     performing bitwise XOR operations and polynomial division.
*                   - Finally, it returns the calculated CRC value.
**********************************************************************
*******************************/
unsigned int CRC_Calculate(const std::vector<unsigned char> &Data);
};
/*****************************************
-----------    Serial Port     -----------
*****************************************/
class Serial_Port : private CRC_Manage ,protected GPIO_Manage
{
/*************** Methods ****************/
protected:
/****************************************************************************************************
* Constructor Name: Serial_Port
* Class           : Serial_Port
* Namespace       : Bootloader
* Description     : Initializes a serial port with specified device location and GPIO pin for control.
* Parameters (in) : Device_Location   - The location of the serial device.
*                   GPIO_Manage_Pin  - The GPIO pin used for managing the serial port.
* Parameters (out): None
* Return value    : None
* Notes           : - This constructor initializes the serial port with specified parameters such as baud rate,
*                     character size, stop bits, and parity.
*                   - It also initializes the GPIO management for controlling the serial port.
*****************************************************************************************************/
Serial_Port(const std::string& Device_Location,const std::string &GPIO_Manage_Pin);
/****************************************************************************************************
* Function Name   : Send_Data
* Class           : Serial_Port
* Namespace       : Bootloader
* Description     : Writes a single byte of data to the serial port.
* Parameters (in) : Data - The single byte of data to be written.
* Parameters (out): None
* Return value    : None
* Notes           : - This function writes a single byte of data to the serial port using asynchronous write operation.
*                   - If ENABLE_DEBUG is defined, it prints debugging information about the sent frame.
*****************************************************************************************************/
void Send_Data(const unsigned char Data);
/****************************************************************************************************
* Function Name   : Receive_Data
* Class           : Serial_Port
* Namespace       : Bootloader
* Description     : Reads a single byte of data from the serial port.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Unsigned char representing the read byte of data.
* Notes           : - This function reads a single byte of data from the serial port using asynchronous read operation.
*                   - If ENABLE_DEBUG is defined, it prints debugging information about the received frame.
*****************************************************************************************************/
bool Receive_Data(unsigned char &Data);
/****************************************************************************************************
* Function Name   : Receive_Data
* Class           : Serial_Port
* Namespace       : Bootloader
* Description     : Reads data from the serial port into the data buffer.
* Parameters (in) : Size - The size of data to be read from the serial port.
* Parameters (out): None
* Return value    : None
* Notes           : - This function resizes the data buffer to the specified size, reads data from the serial port,
*                     and then reverses the order of bytes in the buffer.
*                   - If ENABLE_DEBUG is defined, it prints debugging information about the received frame.
*****************************************************************************************************/
void Receive_Data(size_t Size);
/****************************************************************************************************
* Function Name   : Send_Data
* Class           : Serial_Port
* Namespace       : Bootloader
* Description     : Writes data to the serial port after appending data length and CRC.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function inserts the data length at the beginning of the buffer and appends CRC to the data.
*                   - It then writes the data to the serial port using asynchronous write operation.
*                   - If ENABLE_DEBUG is defined, it prints debugging information about the sent frame.
*                   - Finally, it clears the data buffer.
*****************************************************************************************************/
void Send_Data(void);
/*************** Variables **************/
protected:
std::vector<unsigned char> Data_Buffer{};
private:
boost::asio::io_context Input_Output;
boost::asio::serial_port Port;
};
/*****************************************
------------    Services     -------------
*****************************************/
class Services : private Serial_Port
{
private:
enum Bootloader_Command_t 
{
    Bootloader_Command_Get_Help             =(1),
    Bootloader_Command_Get_ID               =(2),
    Bootloader_Command_Get_Version          =(3),
    Bootloader_Command_Erase_Flash          =(4),
    Bootloader_Command_Flash_Application    =(5),
    Bootloader_Command_Address_Jump         =(6),
    Bootloader_Command_Say_Hi               =(7),
    Bootloader_Command_Send_Data           =(8)
};
/*************** Methods ****************/
public:
/****************************************************************************************************
* Constructor Name: Services
* Class           : Services
* Namespace       : Bootloader
* Description     : Initializes services with a serial port and GPIO pin for control.
* Parameters (in) : Device_Location   - The location of the serial device.
*                   GPIO_Manage_Pin  - The GPIO pin used for managing the serial port.
* Parameters (out): None
* Return value    : None
* Notes           : - This constructor initializes services with a serial port and GPIO pin for control.
*                   - It initializes the Serial_Port base class with the provided device location and GPIO pin.
*****************************************************************************************************/
Services(const std::string &Device_Location,const std::string &GPIO_Manage_Pin);
/****************************************************************************************************
* Function Name   : Get_Version
* Class           : Services
* Namespace       : Bootloader
* Description     : Sends a get version command to the controller and retrieves chip ID, major, and minor version.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function sends a get version command to the controller.
*                   - It retrieves chip ID, major, and minor version from the received data buffer and prints them.
*                   - It prints an error message if no acknowledgment is received after sending the get version command.
*****************************************************************************************************/
void Get_Version(void);
bool Get_Version(unsigned int &ID,unsigned int &Major,unsigned int &Minor);
/****************************************************************************************************
* Function Name   : Get_Help
* Class           : Services
* Namespace       : Bootloader
* Description     : Sends a get help command to the controller and prints available commands.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function sends a get help command to the controller.
*                   - It retrieves the list of available commands from the received data buffer and prints them.
*                   - It prints an error message if no acknowledgment is received after sending the get help command.
*****************************************************************************************************/
void Get_Help(void);
/****************************************************************************************************
* Function Name   : Get_ID
* Class           : Services
* Namespace       : Bootloader
* Description     : Sends a Get ID frame to the controller and retrieves the built-in ID.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function sends a Get ID frame to the controller using Send_Frame function.
*                   - It then retrieves the built-in ID from the received data buffer and prints it.
*                   - If no acknowledgment is received, it prints an error message.
*****************************************************************************************************/
void Get_ID(void);
bool Get_ID(std::vector<unsigned int> &Built_ID);
/****************************************************************************************************
* Function Name   : Jump_Address
* Class           : Services
* Namespace       : Bootloader
* Description     : Prompts the user to enter an address in hexadecimal and jumps to that address.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function prompts the user to enter an address in hexadecimal format.
*                   - It then jumps to the entered address.
*****************************************************************************************************/
void Jump_Address(void);
/****************************************************************************************************
* Function Name   : Erase_Flash
* Class           : Services
* Namespace       : Bootloader
* Description     : Prompts the user to enter the start page and the total number of pages to erase,
*                   and then initiates flash erasing.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function prompts the user to enter the start page and the total number of pages
*                     to erase for flashing.
*                   - It then initiates the flash erasing process with the specified start page and number of pages.
*****************************************************************************************************/
void Erase_Flash(void);
/****************************************************************************************************
* Function Name   : Flash_Application
* Class           : Services
* Namespace       : Bootloader
* Description     : Prompts the user to enter the start page and binary file location, and then
*                   flashes the application to the controller.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function prompts the user to enter the start page and binary file location.
*                   - It then reads the binary file and flashes the application to the controller
*                     with the specified start page and binary payload.
*****************************************************************************************************/
void Flash_Application(void);
/****************************************************************************************************
* Function Name   : Jump_Address
* Class           : Services
* Namespace       : Bootloader
* Description     : Sends a jump address command to the controller with the specified address.
* Parameters (in) : Address - The address to jump to.
* Parameters (out): None
* Return value    : None
* Notes           : - This function prepares the address bytes and sends a jump address command to the controller
*                     with the specified address.
*                   - It prints a message if jumping is done successfully, otherwise, it prints an error message
*                     if no acknowledgment is received after sending the jump address.
*****************************************************************************************************/
bool Jump_Address(unsigned int &Address);
/****************************************************************************************************
* Function Name   : Erase_Flash
* Class           : Services
* Namespace       : Bootloader
* Description     : Sends an erase flash command to the controller with the specified start page and
*                   number of pages to erase.
* Parameters (in) : Start_Page   - The start page for erasing.
*                   Pages_Count  - The number of pages to erase.
* Parameters (out): None
* Return value    : None
* Notes           : - This function prepares the data bytes containing the start page and number of pages
*                     to erase and sends an erase flash command to the controller.
*                   - It prints a message if erasing is done successfully, otherwise, it prints an error message
*                     if no acknowledgment is received after sending the erase pages command.
*****************************************************************************************************/
bool Erase_Flash(unsigned int &Start_Page, unsigned int &Pages_Count);
/****************************************************************************************************
* Function Name   : Flash_Application
* Class           : Services
* Namespace       : Bootloader
* Description     : Sends a flash application command to the controller with the specified start page
*                   and binary payload.
* Parameters (in) : Start_Page   - The start page for flashing.
*                   Payload      - The binary payload to be flashed.
* Parameters (out): None
* Return value    : None
* Notes           : - This function prepares the data bytes containing the start page and payload size,
*                     and sends a flash application command to the controller.
*                   - It then sends the payload frames to the controller.
*                   - It prints a message if the payload frame is sent successfully,
*                     otherwise, it prints an error message if an error occurs while sending frames
*                     or if no acknowledgment is received after sending the payload.
*****************************************************************************************************/
bool Flash_Application(unsigned int &Start_Page,std::string &File_Location);

void Write_Data(void);
bool Write_Data(unsigned int &Address,unsigned int &Data);
void Exit_Bootloader(void);
void Start_Target_Bootloader(void);
bool Say_Hi(void);
private:
/****************************************************************************************************
* Function Name   : Get_Acknowledge
* Class           : Services
* Namespace       : Bootloader
* Description     : Reads a byte of data from the serial port and checks if it matches the ACK value.
* Parameters (in) : None
* Parameters (out): None
* Return value    : Boolean indicating whether ACK is received.
* Notes           : - This function reads a byte of data from the serial port and checks if it matches
*                     the ACK value defined in Bootloader_State_ACK.
*****************************************************************************************************/
bool Get_Acknowledge(void);
/****************************************************************************************************
* Function Name   : Update_Buffer
* Class           : Services
* Namespace       : Bootloader
* Description     : Updates the buffer by reading data size and then reading data based on the size.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - This function reads a byte of data to determine the size of the incoming data.
*                   - It then reads data from the serial port based on the determined size and updates the buffer.
*****************************************************************************************************/
void Update_Buffer(void);
/****************************************************************************************************
* Function Name   : Send_Frame
* Class           : Services
* Namespace       : Bootloader
* Description     : Sends a frame with a specified service command to the controller.
* Parameters (in) : Service - The service command to be sent.
* Parameters (out): None
* Return value    : bool - True if the frame is sent successfully and acknowledged, false otherwise.
* Notes           : - This function prepares a frame with the specified service command and sends it to the controller.
*                   - It checks for acknowledgment after sending the frame. If acknowledgment is received,
*                     it updates the data buffer.
*                   - It returns true if the frame is sent successfully and acknowledged, otherwise returns false.
*****************************************************************************************************/
bool Send_Frame(Bootloader_Command_t Service);
/****************************************************************************************************
* Function Name   : Send_Frame
* Class           : Services
* Namespace       : Bootloader
* Description     : Sends data frames to the controller.
* Parameters (in) : Data - The data to be sent in the frames.
* Parameters (out): None
* Return value    : bool - True if all frames are sent successfully and acknowledged, false otherwise.
* Notes           : - This function sends data frames to the controller. It breaks down the data into
*                     frames of maximum 250 bytes and sends them one by one.
*                   - It checks for acknowledgment after sending each frame. If acknowledgment is not
*                     received, it stops sending further frames and returns false.
*                   - It delays between sending each frame to ensure reliable communication.
*****************************************************************************************************/
bool Send_Frame(std::vector<unsigned char> &Data);
/****************************************************************************************************
* Function Name   : Send_Frame
* Class           : Services
* Namespace       : Bootloader
* Description     : Sends a frame with a specified service command and additional data to the controller.
* Parameters (in) : Service - The service command to be sent.
*                   Data    - Additional data to be sent along with the service command.
* Parameters (out): None
* Return value    : bool - True if the frame is sent successfully and acknowledged, false otherwise.
* Notes           : - This function prepares a frame with the specified service command and additional data
*                     and sends it to the controller.
*                   - It checks for acknowledgment after sending the frame. If acknowledgment is received,
*                     it returns true.
*                   - If no acknowledgment is received or an error occurs during transmission, it returns false.
*****************************************************************************************************/
bool Send_Frame(Bootloader_Command_t Service,std::vector<unsigned char> &Data);
/****************************************************************************************************
* Function Name   : Read_File
* Class           : Services
* Namespace       : Bootloader
* Description     : Reads binary file content into a vector.
* Parameters (in) : None
* Parameters (out): Binary_File - Reference to a vector where the file content will be stored.
* Return value    : bool - True if the file is successfully read, false otherwise.
* Notes           : - This function reads the content of a binary file located at File_Location.
*                   - It determines the size of the file, allocates memory in Binary_File accordingly,
*                     and reads the file content into the buffer.
*                   - If successful, it returns true and Binary_File contains the file content.
*                   - If unsuccessful (e.g., file not found or unable to read), it returns false
*                     and Binary_File is cleared.
*****************************************************************************************************/
bool Read_File(std::vector<unsigned char> &Binary_File,std::string &File_Location);
};









/*****************************************
--------------   Monitor   ---------------
*****************************************/
class Monitor
{
/*************** Methods ****************/
public:
Monitor(Services &User_Interface,const std::string &Repository_Path,const std::string &Binary_Location,std::vector<std::string> &Arguments);
void Start_Monitoring();
private:
bool Check_For_Update(void);
void Get_Update(void);
/************** Variables ***************/
private:
std::string Binary_Repository{};
std::string File_Location{};
std::vector<std::string> &Commands;
Services &Interface;
};









/*****************************************
---------    User Interface     ----------
*****************************************/
class User_Interface : private Services , public Monitor
{
/*************** Methods ****************/
public:
/****************************************************************************************************
* Function Name   : User_Interface
* Class           : User_Interface
* Namespace       : Bootloader
* Description     : Constructor for the User_Interface class.
* Parameters (in) : User_Interface_File - The file location for user interface initialization.
*                   GPIO_Manage_Pin    - The GPIO pin for managing user interface operations.
* Parameters (out): None
* Return value    : None
* Notes           : - This constructor initializes the user interface using the provided file location
*                     and GPIO pin for managing user interface operations.
*                   - It initializes the User_Interface class by inheriting from the Services class,
*                     which handles communication with the controller.
*****************************************************************************************************/
User_Interface(const std::string &User_Interface_File,const std::string &GPIO_Manage_Pin,const std::string &Repository_Path,const std::string &Binary_Location,std::vector<std::string> &Arguments);
~User_Interface();
/****************************************************************************************************
* Function Name   : Start_Application
* Class           : User_Interface
* Namespace       : Bootloader
* Description     : Initiates the user interface for the application, providing various options
*                   for user interaction.
* Parameters (in) : None
* Parameters (out): None
* Return value    : None
* Notes           : - The function utilizes a while loop to continuously display options to the user
*                     until the user chooses to exit.
*                   - User input is taken to select different options, and corresponding actions
*                     are executed based on the chosen option.
*****************************************************************************************************/
void Start_Application(void);
void Print_Target_Info(bool State);
};
}
/********************************************************************
 *  END OF FILE:  Bootloader_Host.h
********************************************************************/
#endif
