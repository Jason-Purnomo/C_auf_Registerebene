//Unser Arduino ist der Slave

#include <util/twi.h>
#define Baud 115200
volatile uint8_t state = 0;
volatile uint8_t channel = 0;   //empfangener Kanal

void i2c_init(){
  DDRC &= ~(1 << DDC4) & ~(1 << DDC5);
  PORTC |= (1 << PORTC4) | (1 << PORTC5);   //Pull-Up aktivieren
  TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
  TWAR = (0x42 << 1);     //Slave-Adresse frei wählbar, wir nehmen ein Bsp. 0x42
  //TWBR = 72; nicht nötig, weil unser Arduino der Slave ist
}

void adc_init(){
  ADCSRA = (1 << ADEN) | (1 << ADPS2) |(1 << ADPS1) | (1 << ADPS0);
  ADMUX = (1 << REFS0) | (channel & 0x07);
}

int main(){
  i2c_init();
  adc_init();

  sei();

  while(1){
  }
}

/*
Bei USART: Byte kommt rein -> ISR -> fertig
Bei I2C:
Ereignis 1: Master spricht uns an (Write)  → ISR!
Ereignis 2: Master sendet Kanalnummer      → ISR!
Ereignis 3: Master spricht uns an (Read)   → ISR!
Ereignis 4: Master will 2. Byte            → ISR!
Ereignis 5: Master sagt "fertig" (NACK)    → ISR!
*/

ISR(TWI_vect){
  switch(TWSR & 0xF8){
    //Master hat Slave-Adresse + Write gesendet
    //Unsere Slave-Adresse + Write wurde erkannt
    case 0x60:    //Table 22-4. Status Codes for Slave Receiver Mode
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
      break;
    
    //Master hat Datenbyte gesendet -> Kanalnummer empfangen
    //Datenbyte wurde empfangen, ACK gesendet
    case 0x80:    //Table 22-4. Status Codes for Slave Receiver Mode
      //Kanal speichern
      channel = TWDR; 

      //ADC Kanal einstellen und wandeln
      ADMUX = (1 << REFS0) | (channel & 0x0F);
      ADCSRA |= (1 << ADSC);
      while(ADCSRA & (1 << ADSC));     //warten auf Ende der Wandlung
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
      break;
  
    //Master hat Slave-Adresse + Read gesendet
    //Unsere Slave-Adresse + Read wurde erkannt
    case 0xA8:    //Table 22-5. Status Codes for Slave Transmitter Mode
      TWDR = ADC >> 8;    //High-Byte senden
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
      break;

    //Master erwartet noch 1 Byte
    //Byte gesendet, Master hat ACK gesendet -> will mehr
    case 0xB8:    //Table 22-5. Status Codes for Slave Transmitter Mode
      TWDR = ADC;         //Low-Byte senden
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
      break;

    //Master sendet NACK -> Übertragung fertig
    //Byte gesendet, Master hat NACK gesendet -> fertig
    case 0xC0:    //Table 22-5. Status Codes for Slave Transmitter Mode
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
      break;

    default:
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
      break;
  }
}
