#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define BAUD 19200
#define MYUBRR F_CPU/16/BAUD-1

void UART_init(uint16_t ubrr){
  UBRRL = (uint8_t)(ubrr & 0xFF);
  UBRRH = (uint8_t)(ubrr >> 8);

  UCSRC |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
  UCSRB |= (1<<RXEN) | (1<<TXEN);
}

void UART_putc(char data){
  while (!(UCSRA & (1<<UDRE)));
  UDR = data;
}

void UART_puts(char* s){
  while (*s > 0) UART_putc(*s++);
}

int main(){
  UART_init(MYUBRR);
  char data[] = "CFromZero, data from ATmega16\r\n";
  while (1){
    UART_puts(data);
    _delay_ms(1000);
  }
}
