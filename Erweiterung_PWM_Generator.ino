#include <Arduino.h>

int main() {
  // Output PB1 (Arduino Pin 9)
  DDRB |= (1 << DDB1);

  //Input PC0 & PC1
  DDRC &= ~(1 << DDC0) & ~(1 << DDC1);

  // Fast PWM Mode 14
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);
  // non-inverting
  TCCR1A |= (1 << COM1A1);
  // prescaling = 1
  TCCR1B |= (1 << CS10);

  ICR1 = 16000;
  OCR1A = 8000;

  uint32_t Frequenz, Tastgrad;

  while (1) {
    uint16_t adc_val1 = analogRead(A0);
    Frequenz = ((long)adc_val1 * 500)/1023 + 500;

    ICR1 = (16000000UL / Frequenz) - 1;

    uint16_t adc_val2 = analogRead(A1);
    Tastgrad = ((long)adc_val2 * 100)/1023;

    OCR1A = (Tastgrad * ICR1)/100;
  }
}
