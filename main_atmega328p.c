#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define BAUD 19200
#define MYUBRR F_CPU/16/BAUD-1

void UART_init(uint16_t ubrr){
  UBRR0L = (uint8_t)(ubrr & 0xFF);
  UBRR0H = (uint8_t)(ubrr >> 8);

  UCSR0C = (1<<UCSZ00)|(1<<UCSZ01)|(1<<USBS0);
  UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
}

void UART_putc(unsigned char data){
  while (!(UCSR0A & (1<<UDRE0)));
  UDR0 = data;
}

void UART_puts(char* s){
  while (*s > 0) UART_putc(*s++);
}

int main(){
  UART_init(MYUBRR);
  int i;
  unsigned char data[] = "CFromZero, USART-ATmega328p\r\n";
  while (1){
    UART_puts(data);
    _delay_ms(1000);
  }
}
