int main(){ 
  uint16_t x;
  
  //AD-Wandler konfigurieren
  ADCSRA |= (1 << ADEN);
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADMUX |= (1 << MUX1) | (1 << MUX0);
  ADMUX |= (1 << REFS0);

  //SPI Master konfigurieren
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

  //PINs konfigurieren
  DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
  DDRB &= ~(1 << DDB4);
  PORTB |= (1 << PORTB2);

  sei();

  while(1){
    ADCSRA |= (1 << ADSC);          //AD-Wandler Starten
    while(ADCSRA & (1 << ADSC));    //Auf Ende der Wandlung warten
    x = ADC;

    //Problem: x ist uint16_t = 16Bit = 2Byte, aber SPI überträgt immer 1Byte auf einmal
    PORTB &= ~(1 << PORTB2);        //SS LOW setzen
    SPDR = (x >> 8);                //High-Byte senden
    while(!(SPSR & (1 << SPIF)));   //Auf Ende SPI Transfer warten
    SPDR = (x & 0xFF);              //Low-Byte senden
    while(!(SPSR & (1 << SPIF)));   //Auf Ende SPI Transfer warten
    PORTB |= (1 << PORTB2);         //SS HIGH setzen
  }
}
