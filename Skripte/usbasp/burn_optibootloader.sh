#!/bin/bash

avrdude -p m328p -c usbasp -U flash:w:optiboot_atmega328.hex:i -F -P usb

