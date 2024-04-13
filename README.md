# OTA Update Implementation with Arm-Based Controller, Raspberry Pi 4.

https://github.com/t0ti20/FOTA/assets/61616031/31f67687-9324-4508-b5bc-6cffe37c6a84

## Overview

This project focuses on implementing Over-the-Air (OTA) updates utilizing an Arm-Based Controller, a Raspberry Pi 4, and GitHub as the cloud storage platform. It aims to enable seamless remote software updates for connected devices, enhancing device management and maintenance efficiency.

## Features

### Arm-Based Controller:

- **Bootloader-Target Application**: Manages bootloader commands and interface.
- **Main Application**: Operates in normal mode but can switch to bootloader mode upon command from the Raspberry Pi.

### Raspberry Pi 4:

- Utilizes the Yocto build system to create a custom image.
- Configures the kernel and Wi-Fi to connect to the network.
- Boots from the network file system for development purposes.
- Implements a C++ host-bootloader for managing commands.

### GitHub Cloud:

- Serves as the cloud storage platform for OTA updates.
- Developers can commit new application binaries to GitHub.
- The Raspberry Pi automatically detects commits, downloads the binaries, and flashes them onto targeted devices.

## Key Points

- Enables seamless OTA updates using Arm-Based Controller, Raspberry Pi 4, and GitHub.
- Facilitates efficient remote software updates for connected devices.
- Welcomes community contributions, insights, and ideas for further enhancements.


## Videos

- CLI Commands Testing Between Raspberry Pi and Arm Controller
  
https://github.com/t0ti20/FOTA/assets/61616031/7854995b-27e7-4165-9922-8187919d4c36

- Automatic Update Sensing and Downloading Final Image

https://github.com/t0ti20/FOTA/assets/61616031/26c402e1-4767-473d-9e57-bfa89e598f33

- Idea After Implementation

https://github.com/t0ti20/FOTA/assets/61616031/b4639e5f-441c-4a55-b21f-0d996178520e


## How to Contribute

Contributions to enhance the project are welcome. Feel free to submit pull requests or share your ideas through the issue tracker.
