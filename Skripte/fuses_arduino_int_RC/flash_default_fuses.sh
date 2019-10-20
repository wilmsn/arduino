#!/bin/bash

avrdude -c usbasp -P usb -B 10 -p atmega328P -U lfuse:w:0xFF:m -U hfuse:w:0xDA:m -U efuse:w:0x05:m
