#ifndef SWITCH_PROGRAM_BUZZER_H
#define SWITCH_PROGRAM_BUZZER_H

#include <Arduino.h>

#define BUZZER_PIN 33 // Ganti dengan pin yang sesuai

void playSwitchProgramSound() {
  tone(BUZZER_PIN, 660, 150);  // Nada 1
  delay(150);
  noTone(BUZZER_PIN);
  delay(50);
  tone(BUZZER_PIN, 880, 150);  // Nada 2
  delay(150);
  noTone(BUZZER_PIN);
  delay(50);
  tone(BUZZER_PIN, 1100, 300); // Nada 3 (lebih panjang)
  delay(300);
  noTone(BUZZER_PIN);
}

#endif