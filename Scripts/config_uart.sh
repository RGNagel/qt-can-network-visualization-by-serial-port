#!/bin/bash

BAUD=57600
DEV=/dev/ttyUSB0

# ref: https://stackoverflow.com/questions/41266001/screen-dev-ttyusb0-with-different-options-such-as-databit-parity-etc
# stty - change and print terminal line settings
#
#    -F /dev/ttyUSB0      Change the settings of /dev/ttyUSB0
#    cs8                  Use 8 character bits
#    -parenb              Don't use a parity bit (the '-' means 'disable')
#    crtscts              Enable RTS/CTS handshaking (hardware flow control)
sudo stty -F $DEV cs8 -parenb

screen $DEV $BAUD
#sudo picocom $DEV --baud $BAUD --parity n --databits 8 --nolock
#sudo picocom $DEV -b $BAUD
