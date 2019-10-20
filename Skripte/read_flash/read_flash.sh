#/bin/bash

avrdude  -B 10 -c usbasp -p atmega328P -U flash:r:flash.hex:i
