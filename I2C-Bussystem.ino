bool checkTWIAddress(uint8_t address){
  //Start senden
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  //warten
  while(!(TWCR & (1 << TWINT)));

  //status überprüfen: start wurde gesendet
  if(TW_STATUS != TW_START){
    return false;
  }

  //Adresse senden
  TWDR = address << 1;
  TWCR = (1 << TWINT) | (1 << TWEN);
  //warten
  while(!(TWCR & (1 << TWINT)));

  //status überprüfen: slave hat adresse bestätigt
  bool found = false;
  if(TW_STATUS == TW_MT_SLA_ACK){
    found = true;
  }

  //Stop senden
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

  return found;
}

int main() {
  Serial.begin(9600);

  TWBR = 72;

  for (int i = 1; i <= 127; i++){
    if (checkTWIAddress(i)) {
      Serial.print("Slave found: ");
      Serial.println(i);
    }
  }
  while (1);
}
