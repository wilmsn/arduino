#!/bin/bash
#
# Einstellung ohne BOD
#
avrdude -c usbasp -P usb -B 10 -p atmega328P -U lfuse:w:0xE2:m -U hfuse:w:0xDB:m -U efuse:w:0xFF:m

echo -n "Hit <Return> to continue"
read xxxx

