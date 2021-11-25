# AVR-USART
Example codes for AVR-GCC and ATmega328p, 8 and 16

make
avrdude -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -Uflash:w:main.hex
avrdude -patmega328p -cusbasp -Pusb -Uflash:w:main.hex

