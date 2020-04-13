#!/bin/bash
#
# HEX Datei als Parameter
#
if [ -n "${1}" ]; then
avrdude -c usbasp -P usb -B 10 -p atmega328P -U flash:w:${1}
else
echo "Usage: ${0} <Programm as HEX file>"
fi
echo -n "Hit <Return> to continue"
read xxxx

