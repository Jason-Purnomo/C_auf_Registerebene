#include <util/twi.h>
int main() {
  uint8_t data;

  //Pull-Up aktivieren, SDA & SCL
  PORTC |= (1 << PORTC4) | (1 << PORTC5);

  TWBR = 72;

  //Start senden
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);  

  // Warten
  while (!(TWCR & (1 << TWINT)));

  // Adresse Slave A senden
  TWDR = (7 << 1) | 1;
  TWCR = (1 << TWINT) | (1 << TWEN);

  // Warten
  while (!(TWCR & (1 << TWINT)));

  // Empfang starten
  TWCR = (1 << TWINT) | (1 << TWEN);
  /*Mit TWEA → Master sendet ACK  → „schick mir noch ein Byte"
    Ohne TWEA → Master sendet NACK → „das ist das letzte Byte" */

  // Warten
  while (!(TWCR & (1 << TWINT)));

  // Daten auslesen
  data = TWDR;

  //Stop senden
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

  //Start senden
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

  // Warten
  while (!(TWCR & (1 << TWINT)));

  // Adresse Slave B senden
  TWDR = (8 << 1) | 1;
  TWCR = (1 << TWINT) | (1 << TWEN);

  // Warten
  while (!(TWCR & (1 << TWINT)));

  // Daten senden
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);

  // Warten
  while (!(TWCR & (1 << TWINT)));

  //Stop senden
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

  while(1);  
}
