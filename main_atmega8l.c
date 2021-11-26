// ATmega8L
#define F_CPU 8000000

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define BAUD 19200
#define MYUBRR F_CPU/16/BAUD-1

void UART_init(uint16_t ubrr){
  UBRRL = (unsigned char)(ubrr);
  UBRRH = (unsigned char)(ubrr >> 8);

  UCSRB = (1<<RXEN) | (1<<TXEN);
  UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
}

void UART_putc(unsigned char data){
  while (!(UCSRA & (1<<UDRE)));
  UDR = data;
}

void UART_puts(unsigned char* s){
  while (*s > 0) UART_putc(*s++);
}

int main(void){
  UART_init(MYUBRR);
  unsigned char data[] = "CFromZero, data from ATmega8L\r\n";
  while (1){
    UART_puts(data);
    _delay_ms(1000);
  }
}
