# AVR-USART
Example codes for AVR-GCC and ATmega328p, 8 and 16

make

avrdude -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -Uflash:w:main.hex

avrdude -patmega328p -cusbasp -Pusb -Uflash:w:main.hex

Note:

/*

 * Set frame format: 1000 0110 (URSEL UMSEL UPM1 UPM0 USBS UCSZ1 UCSZ0 UCPOL)
 
 * Asynchronous mode (UMSEL = 0), Parity: None (UPM1 = 0, UPM0 = 0), Stop bit(s): 1 (USBS = 0)
 
 * Character Size: 8-bit
 
 * 

*/

// 8,N,1 

UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0); //atmega8l

UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0); //atmega16

/* 

 * Set frame format: 0000 1110 (URSELn1 UMSELn0 UPMn1 UPMn0 USBSn UCSZn1 UCSZn0 UCPOLn)

 * Asynchronous mode (UMSEL = 0), Parity: None (UPMn1 = 0, UPMn0 = 0), Stop bit(s): 2 (USBSn = 1)
 
 * Character Size: 8-bit (UCSZn1 = 1, UCSZn0 = 1), Rising XCKn edge (UCPOLn = 0)

*/

// 8,N,2

UCSR0C = (1<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00);  //atmega328p

