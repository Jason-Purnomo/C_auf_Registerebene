#include <avr/io.h>
volatile int Zahl = 0;
volatile uint8_t pind_old = 0;  //LOW
volatile uint8_t pinb_old = 0;  //LOW

int main(){
  //Pin konfigurieren
  DDRD &= ~(1 << DDD7);
  DDRB &= ~(1 << DDB0);
  // Pull-Up aktivieren -> Wenn Taster nicht gedrückt ist, liegt am PIN HIGH
  PORTD |= (1 << PORTD7);
  PORTB |= (1 << PORTB0);

  //Interrupts konfigurieren
  PCICR |= (1 << PCIE0) | (1 << PCIE2);
  PCMSK0 |= (1 << PCINT0);
  PCMSK2 |= (1 << PCINT23);

  //Anfangszustände speichern
  pind_old = PIND;
  pinb_old = PINB;

  sei();

  while(1);
}

ISR(PCINT0_vect){
  uint8_t current = PINB;                 //aktuellen Port-Zustand lesen
  uint8_t change = current ^ pinb_old;    //gib eine 1 an jeder Stelle, wo sich ein Bit geändert hat
  if((change & (1 << PINB0)) && !(current & (1 << PINB0))){    //fallende Flanke
    Zahl--;
  }
  pinb_old = current;
}
ISR(PCINT2_vect){
  uint8_t current = PIND;                 //aktuellen Port-Zustand lesen
  uint8_t change = current ^ pind_old;    //gib eine 1 an jeder Stelle, wo sich ein Bit geändert hat
  if((change & (1 << PIND7)) && (current & (1 << PIND7))){    //steigende Flanke
    Zahl++;
  }
  pind_old = current;
}
