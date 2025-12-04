#include <util/twi.h>

int main(void){
  init();
  Serial.begin(9600);

  //Pull-Up aktivieren -> zur Vermeidung von Kurzschlüssen
  PORTC |= (1 << PORTC4) | (1 << PORTC5);       //SDA & SCL

  //TWAR bei Master-Modus nicht gebraucht.
  TWBR = 72;      //fSCL = 100kHz
  //oder TWBR = (1 << TWBR6) | (1 << TWBR3);

  uint8_t data_in = 0;

  while(1){
    //Start senden
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    //warten bis Start-Befehl fertig
    while(!(TWCR & (1 << TWINT)));

    //Status überprüfen: Start wurde gesendet
    if(TW_STATUS != TW_START){
      ERROR();
    }

    //Adresse senden
    uint8_t SLA_W = 1;       
    TWDR = SLA_W << 1;        //setzt Slave Adresse (SLA6...SLA0)
    TWDR |= (1 << 0);         //Bit Richtung = Read = 1;

    //Adress Frame senden
    TWCR = (1 << TWINT) | (1 << TWEN)

    //warten
    while(!(TWCR & (1 << TWINT)));

    //Status überprüfen: Slave hat Adresse Bestätigt
    if(TW_STATUS != TW_MR_SLA_ACK){
      ERROR();
    }

    //Daten auslesen
    data_in = TWDR;
    float Liter = data_in * 5.0/256.0;
    Serial.print("Daten: ");
    Serial.println(Liter);

    //Stop senden
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
  }
}

void ERROR(){
  Serial.print("TWCR: ");
  Serial.println(TWCR, BIN);
  Serial.print("TW_STATUS: ");
  Serial.println(TW_STATUS, HEX);
}
