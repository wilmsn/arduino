#!/bin/bash

avrdude  -c usbasp -p atmega8 -U usbasp.atmega8.2011-05-28.hex
