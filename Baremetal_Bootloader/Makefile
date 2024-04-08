##################################################
# *  FILE DESCRIPTION
#########################
# *  Author:  Khaled El-Sayed (@t0ti20)
# *  Created:  05.02.2023
# *  File:  Makefile
# *  Module:  Global
# *  Description:  Make File For Auto Generation
##################################################
#Select Name Of Application
Application=Baremetal_Bootloader
#Build Directory
Build=./Build
#Compiler Name
CC=arm-none-eabi-
#Compiler Flags
CFLAGS = -O0 -gdwarf-4 -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections -g
#Asseember Flags
AFLAGS= -gdwarf-4 -g
#Linker Flags
LFLAGS= -O0 -gdwarf-4 --std=gnu99 -mthumb -mcpu=cortex-m3 --specs=nosys.specs -Wl,--gc-sections -g
#External Include Directories
INCS=
# INCS+=-I /usr/lib/arm-none-eabi/include
#External Liberaries
LIBS=
# LIBS+=-L /usr/lib/arm-none-eabi/lib
# LIBS+=-L /usr/lib/arm-none-eabi/newlib
#Flash Memory
FLASH_Address = 0x08000000
##################################################
################### Variables ####################
##################################################
SRC=$(shell find $(SOURCEDIR) -name '*.c')
SRS=$(shell find $(SOURCEDIR) -name '*.s')
SRL=$(shell find $(SOURCEDIR) -name '*.ld')
INCS+=$(shell find $(SOURCEDIR) -name '*.h' -exec dirname {} \; | uniq | sed 's/^/-I /')
OBJ=$(SRC:.c=.o) $(SRS:.s=.o)	
##################################################
################# Main Methouds ##################
##################################################
.PHONY: clean
all: $(Application).hex $(Application).bin
	@echo "========== Build Finished ========="
#*******************************************************************
#*******************************************************************
#Compile And Assimble C Files Files
%.o:%.c
	@$(CC)gcc $(CFLAGS) -c $< $(INCS) -o $(Build)/$(notdir $@)
#*******************************************************************
#*******************************************************************
#Assimble Assimply Files
%.o:%.s
	@$(CC)as $(AFLAGS) $< -o $(Build)/$(notdir $@)
#*******************************************************************
#*******************************************************************
#Link All Files And Generate ELF File
$(Application).elf: $(OBJ)
	@$(CC)gcc $(Build)/*.o $(INCS) $(LFLAGS) -T $(SRL) $(LIBS) -o $(Build)/$@ 
#*******************************************************************
#*******************************************************************
#Generate Hex File
$(Application).hex: $(Application).elf
	@$(CC)objcopy -O ihex $(Build)/$< $(Build)/$@ 
	@cp $(Build)/$(Application).elf $(Build)/$(Application).axf 
#*******************************************************************
#*******************************************************************
#Generate Binary File
$(Application).bin: $(Application).elf
	@$(CC)objcopy -O binary $(Build)/$< $(Build)/$@ 
#*******************************************************************
#*******************************************************************
#Clean All Files
.PHONY: clean
clean: 
	@rm -f -r $(Build)/*
	@echo "========= Cleaning Finished ========"
#*******************************************************************
#*******************************************************************
#Flash On Board
.PHONY: flash
flash: all
	@if [ ! -w /dev/bus/usb ];then sudo chmod -R 777 /dev/bus/usb;fi
	@st-flash write $(Build)/$(Application).bin ${FLASH_Address} > /dev/null 2>&1
	@echo "========= Flashing Finished ======="
#*******************************************************************
#*******************************************************************
#Erase The Flash 
.PHONY: erase
erase:
	@sudo chmod -R 777 /dev/
	@st-flash erase > /dev/null 2>&1
	@echo "========= Erasing Finished ========"
#*******************************************************************
#*******************************************************************
FLASH_Page_Size_Bytes = 1024
Total_Pages = 127
# Read The Flash 
.PHONY: read do_read
read:
	@make -C . do_read SIZE_PAGES=$(Total_Pages)
do_read:
	@sudo chmod -R 777 ./Build/
	@st-flash read ./Build/FLASH.bin 0x08000000 $(shell expr $(FLASH_Page_Size_Bytes) \* $(Total_Pages)) > /dev/null 2>&1
	@echo "========= Reading Finished ========"
#*******************************************************************
#*******************************************************************