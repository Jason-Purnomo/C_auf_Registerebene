void sendSerialPin4(uint32_t baud, uint8_t b){
  //Baudrate = [Bits/Sekunde]
  //Bitdauer in mikrosekunden (1s ~ 1000000us)
  uint16_t bit_us = (uint16_t)1000000UL / baud;
  //Interrupts sperren/deaktivieren
  uint8_t sreg = SREG;
  cli();      //global Interrupt disable
  //idle Zustand: Leitung HIGH
  PORTD |= (1 << PORTD4);

  //Start Übertragung
  //Startbit: LOW
  PORTD &= ~(1 << PORTD4);
  _delay_us(bit_us);

  //8 Datenbits, LSB First
  for(uint8_t i = 0; i < 8; i++){
    if(b & 0x01){
      PORTD |= (1 << PORTD4);   //Bit = 1
    } else {
      PORTD &= ~(1 << PORTD4);  //Bit = 0
    }
    _delay_us(bit_us);
  }

  //Stopbit: HIGH
  PORTD |= (1 << PORTD4);
  _delay_us(bit_us);

  //Ende der Übertragung
  SREG = sreg;
}

int main() {
  // Pin 4 PD4 als Ausgang
  DDRD |= 1 << DDD4;
  
  while (1) {
    uint8_t b = 0x4C;
    sendSerialPin4(38400, b);
    _delay_ms(1000);
  }
}
