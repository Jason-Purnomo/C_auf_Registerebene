//Arduino: Master ; Externe Baustein: Slave

int main(){
  uint8_t ID;     //Hersteller-ID
  uint16_t SN;    //Seriennummer
  uint8_t V;      //Version

  //Pin konfigurieren
  DDDB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
  DDDB &= ~(1 << DDB4);
  PORTB |= (1 << PORTB2);   //Setzen SS auf HIGH

  //SPI konfigurieren
  SPCR = (1 << SPE) | (1 << MSTR);
  //SPCR = (1 << SPR0)    --> Optional, um Taktteiler auf 1MHZ zu setzen (sinnvolle Schutzmaßnahme), sonst 4MHz kann zu Probleme führen
  PORTB &= ~(1 << PORTB2);   //Setzen SS auf LOW

  //Byte 1 empfangen
  SPDR = 0x00;    //Dummy-Wert, Takt erzeugen --> starts transmission
  while(!(SPSR & (1 << SPIF)));   //warten auf Ende SPI Transfer
  uint8_t byte1 = SPDR;       //Data lesen / empfangen
 
  //Byte 2 empfangen
  SPDR = 0x00;    //Dummy-Wert, Takt erzeugen --> starts transmission
  while(!(SPSR & (1 << SPIF)));   //warten auf Ende SPI Transfer
  uint8_t byte2 = SPDR;       //Data lesen / empfangen

  PORTB |= (1 << PORTB2);   //Setzen SS auf HIGH

  //Bits extrahieren
  ID = (byte1 >> 4);
  SN = ((uint16_t)(byte1 & 0x0F) << 6) | ((byte2 & 0xFC) >> 2);    //0x0F = 00001111 ; 0xFC = 11111100
  V = (byte2 & 0x03);   //0x03 = 00000011

  while(1){
  }
}
