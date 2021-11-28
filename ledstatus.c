#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define BAUD 1200
#define MYUBRR F_CPU/16/BAUD-1

void UART_init(uint16_t ubrr){
  UBRRL = (uint8_t)(ubrr);
  UBRRH = (uint8_t)(ubrr>>8);
  UCSRB = (1<<RXEN) | (1<<TXEN);
  UCSRC = (1<<URSEL) | (3<<UCSZ0);
}

unsigned char UART_RxChar(){
  while ((UCSRA & (1<<RXC)) == 0);
  return (UDR);
}

void UART_TxChar(char ch){
  while (!(UCSRA & (1<<UDRE)));
  UDR = ch;
}

void UART_SendString(char *str){
  unsigned char j = 0;
  while (str[j] != 0){
    UART_TxChar(str[j]);
    ++j;
  }
}

int main(void){
  int i = 0, direct = 0;
  uint8_t j = 0, stop = 0;
  DDRB = 0xFF;
  PORTB = 0xFF;
  UART_init(MYUBRR);
  while (stop == 0){
    if(PIND & (1<<PIN7)){
      UART_SendString("!q!\n");
      stop = (stop) ? 0 : 1;
    }else{
      PORTB ^= 1<<i;
      for (j = 0; j <= 7; ++j)
        (PINB & (1<<j)) ? UART_TxChar('1') : UART_TxChar('0');
      UART_SendString("\n");
      _delay_ms(50);
      PORTB |= 1<<i;
      _delay_ms(50);
      if (direct == 0){
        ++i;
        if (i > 7){
          i=7;
          direct = 1;
        }
      }else{
        if (i < 0){
          i = 0;
          direct = 0;
        }
      }
    }
  }
  return 0;
}
