#define Baud 115200

void usart_init(){
  UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

  //PIN konfigurieren
  DDRD &= ~(1 << DDD0);
  DDRD |= (1 << DDD1);

  UBRR0 = (16000000UL / 16*Baud) - 1;     // = 8
}

void adc_init(){
  ADCSRA = (1 << ADEN) | (1 << ADPS2) |(1 << ADPS1) | (1 << ADPS0);
  ADMUX = (1 << REFS0) | (channel & 0x07);
}

int main(){
  usart_init();
  adc_init();

  sei();

  while(1){
  }
}

ISR(USART_RX_vect){
  //1. Kanal empfangen
  uint8_t channel = UDR0;

  //2. Kanal im ADC einstellen
  ADMUX = (1 << REFS0) | (channel & 0x0f);
  //(channel & 0x0f) --> Kanäle 0-8

  //3. Wandlung starten
  ADCSRA |= (1 << ADSC);

  //warten auf Ende der Wandlung
  while(ADCSRA & (1 << ADSC));

  //4. Ergebnis über USART senden
  //ADC liefert ein 10-bit Ergebnis (0...1023), aber USART kann immer nur 1 Byte auf einmal senden
  //High-Byte zuerst
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = (ADC >> 8);

  //Low-Byte
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = ADC;
}
