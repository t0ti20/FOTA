#!/bin/bash
############################################
#----------- User Configuration ------------
############################################
SERIAL_PORT="/dev/ttyUSB0"
BAUD_RATE="9600"
LOG_FILE="Bootloader.log"
SESSION_NAME="Bootloader_Communication"
# Trap to handle cleanup on exit
trap cleanup EXIT
############################################
#------------- Main Functions --------------
############################################
############################################
cleanup() 
{
    echo "Exiting and closing screen session..."
    screen -S $SESSION_NAME -X quit
    rm -f $LOG_FILE
    echo "Exited."
}
############################################

############################################
############################################
send_hex() {
    local hex_data="$1"
    echo -n -e $(echo $hex_data | sed 's/\([0-9A-F]\{2\}\)/\\x\1/gI') | socat - $SERIAL_PORT,raw,echo=0
}
############################################
compute_crc32()
{
    local data="$1"
    CRC_Value=$(python3 -c "
from crccheck.crc import Crc32Mpeg2
data_bytes = bytes.fromhex('$data')
crc = Crc32Mpeg2.calc(data_bytes)
print(f'{crc:08X}')
")
    # Return the concatenated value of the input data and its CRC
    echo "${data}${CRC_Value}"
}
Get_Help()
{
     Frame_Size="02"
     Get_Help_Frame="${Frame_Size}$(compute_crc32 "00000001")"
     echo $Get_Help_Frame
     send_hex $Get_Help_Frame
     Receive_Frame
}
Receive_Frame()
{
    # Clear the LOG_FILE content
    > $LOG_FILE

    # Wait for the sender to finish. You might want to adjust this value.
    sleep 2

    # Read the LOG_FILE and output its content in hex format
    received_bytes=$(cat $LOG_FILE | xxd -p)

    # Check if the received_bytes are empty
    if [ -z "$received_bytes" ]; then
        echo "Error: No data received or log file is empty."
        return 1
    fi

    echo "Received bytes: $received_bytes"

    # Return success
    return 0
}


############################################
#------------ Main Application -------------
############################################
echo "Starting Session ..."
screen -dm -S $SESSION_NAME -L -Logfile $LOG_FILE $SERIAL_PORT $BAUD_RATE
# Allow screen some time to initialize
sleep 1
while true;do
     echo "Choose an option:"
     select opt in "Get Help." "Get Chip Version." "Get ID." "Get Version." "Erase Code Flash." "Write Code Flash." "Exit";do
     case $opt in
          "Get Help.")
               Get_Help
               break;;
          "Get ID.")
               break;;
          "Get Chip Version.")
               break;;
          "Get Version.")
               break;;
          "Erase Code Flash.")
               break;;
          "Write Code Flash.")
               break;;
          "Exit")
               exit 0;;
          *)
               echo "Invalid option. Please choose again.";;
        esac
    done
done