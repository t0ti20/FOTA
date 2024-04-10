/*******************************************************************
 *  FILE DESCRIPTION
-----------------------
 *  Author: Khaled El-Sayed @t0ti20
 *  File: Bootloader.cpp
 *  Date: March 28, 2024
 *  Description: Application For Bootloader
 *  (C) 2023 "@t0ti20". All rights reserved.
*******************************************************************/
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "Bootloader_Interface.hpp"
/*****************************************
---------    Configurations     ----------
*****************************************/
constexpr const char GPIO_Pin[]                 {"18"};
constexpr const char Serial_Driver[]            {"/dev/ttyS0"};
constexpr const char Binary_Repo[]              {"/home/root/FOTA"};
constexpr const char Binary_File[]              {"/Application/Build/Blink.bin"};
/*****************************************
----------   Main Application   ----------
*****************************************/
int main(int argc, char* argv[]) 
{
    using namespace Bootloader;
    std::vector<std::string> Arguments;
    User_Interface Application
    {
        Serial_Driver,
        GPIO_Pin,
        Binary_Repo,
        std::string(Binary_Repo)+std::string(Binary_File),
        Arguments
    };
    for (size_t Counter=1;Counter<argc;++Counter){Arguments.emplace_back(argv[Counter]);}
    if(Arguments.size())
    {
        Application.Start_Monitoring();
    }
    else
    {
        if(system("clear")){};
        Application.Start_Application();
    }
    return 0;
}
/********************************************************************
 *  END OF FILE:  Bootloader.cpp 
********************************************************************/