#include "SevSeg.h"
int number=0;
// Piny cyfr (common pins)
const byte digitPins[] = {10,11, 12, 13}; // Przykładowe piny

// Piny rejestru przesuwnego (SER, SRCLK, RCLK)
const byte shiftRegisterPins[] = {7, 8, 9}; // Przykładowe piny

SevSeg sevseg;

void setup() {
  sevseg.begin(COMMON_ANODE,4, digitPins, shiftRegisterPins);
  sevseg.setBrightness(90);
}

void loop() {
  sevseg.setNumber(number);
  for(int i = 0; i < 100; i++) {
    sevseg.refreshDisplay();
    delay(1);
  }
  sevseg.refreshDisplay();
  number++;
}
