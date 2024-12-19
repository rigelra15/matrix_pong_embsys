#ifndef BLUETOOTH_BUZZER_H
#define BLUETOOTH_BUZZER_H

#include <Arduino.h>

#define BUZZER_PIN 33 

void playBluetoothConnectSound() {
  tone(BUZZER_PIN, 880, 200);  
  delay(200);
  noTone(BUZZER_PIN);
  delay(100);
  tone(BUZZER_PIN, 880, 200);
  delay(200);
  noTone(BUZZER_PIN);
}

void playBluetoothDisconnectSound() {
  tone(BUZZER_PIN, 440, 200);  
  delay(200);
  noTone(BUZZER_PIN);
  delay(100);
  tone(BUZZER_PIN, 440, 200);
  delay(200);
  noTone(BUZZER_PIN);
}

#endif