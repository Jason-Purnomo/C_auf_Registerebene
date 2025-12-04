#include <util/twi.h>

int main(void){
  init();
  Serial.begin(9600);

  //Konfiguration
  DDRC &= ~(1 << DDC0);     //POTI

  //ADC-Wandler
  ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRB = 0;
  ADMUX = (1 << REFS0) | 0;

  ADCSRA |= (1 << ADSC);        //ADC-Wandler starten

  //Slave Adresse
  uint8_t SLA_W = 1;
  TWAR = (SLA_W << 1);

  //Slave aktivieren
  TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);

  while(1){
    //warten
    while(!(TWCR & (1 << TWINT)));

    //Status überprüfen: Eigene Adresse empfangen
    if(TW_STATUS != TW_ST_SLA_ACK){
      ERROR();
    }

    //Daten senden
    uint16_t tank_level = ADC;
    Serial.print(tank_level);
    TWDR = (tank_level >> 2);

    TWCR = (1 << TWINT) | (1 << TWEN);

    //warten
    while(!(TWCR & (1 << TWINT)));

    //Slave aktivieren
    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
  }
}

void ERROR(){
  Serial.print("TWCR: ");
  Serial.println(TWCR, BIN);
  Serial.print("TW_STATUS: ");
  Serial.println(TW_STATUS, HEX);
}