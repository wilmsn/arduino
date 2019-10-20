D:
rem
rem Einstellung bis 29.05.2014 (ohne BOD)
rem
rem \PortableAPPS\arduino-1.0.5\hardware\tools\avr\bin\avrdude  -C \PortableAPPS\arduino-1.0.5\hardware\tools\avr\etc\avrdude.conf -c usbasp -P usb -B 10 -p atmega328P -U lfuse:w:0xE2:m -U hfuse:w:0xDA:m -U efuse:w:0x07:m
rem
rem Einstellung BOD 2,7V ohne Bootloaderschutz
rem
\PortableAPPS\arduino-1.0.5\hardware\tools\avr\bin\avrdude  -C \PortableAPPS\arduino-1.0.5\hardware\tools\avr\etc\avrdude.conf -c usbasp -P usb -B 10 -p atmega328P -U lfuse:w:0xE2:m -U hfuse:w:0xDF:m -U efuse:w:0x05:m
pause
