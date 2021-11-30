#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define BAUD 1200
#define MYUBRR F_CPU/16/BAUD-1

#define BUTTON_MASK	(1<<PD2)
#define BUTTON_PIN 	PIND
#define BUTTON_PORT 	PORTD

#define LED_MASK 	(1<<PC0)
#define LED_PORT 	PORTC
#define LED_DDR  	DDRC

volatile uint8_t button_down;

void debounce(void){
  uint8_t count = 0;
  uint8_t button_state = 0;
  uint8_t current_state = (~BUTTON_PIN & BUTTON_MASK) != 0;
  if (current_state != button_state){
    count++;
    if (count>=10){
      button_state = current_state;
      if (current_state != 0)
        button_down = 1;
      count = 0;
    }
  }else
    count = 0;
}

uint8_t swap = 0;
ISR(INT0_vect){
  //debounce();
  swap = 1;
}

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
  //MCUCR &= ~(1<<ISC01) | (1<<ISC00); 	//Pull Up Register, Low		0, 0
  //MCUCR = (0<<ISC01) | (1<<ISC00); 	//Pull down Register, Down-Rise	0, 1
  //MCUCR = (1<<ISC01) | (0<<ISC00); 	//Pull down Register, Down	1, 0
  MCUCR = (1<<ISC01) | (1<<ISC00); 	//Pull down Register, Rise	1, 1
  GICR |= (1<<INT0);
  sei();

  DDRD = 0x00;
  PORTD = (1<<PD2);

  DDRC = (1<<DDC0);
  PORTC = (0<<DDC0);

  UART_init(MYUBRR);

  while(1){
    //UART_SendString("...\n");

    if (swap == 1){
      PORTC ^= (1<<DDC0);
      GICR  &= ~(1<<INT0);
      _delay_ms(100);
      while (!(PIND & (1<<PD2))){UART_SendString("...\n");}
      _delay_ms(100);
      swap = 0;
    }

    set_sleep_mode(SLEEP_MODE_IDLE);
    GICR |= (1<<INT0);
    sleep_mode();

  }

  return 0;
}
