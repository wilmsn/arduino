#/bin/bash

avrdude  -B 10 -c usbasp -p atmega328P -U flash:w:flash.hex
