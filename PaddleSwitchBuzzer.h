#ifndef PADDLEBUZZER_H
#define PADDLEBUZZER_H

#include <Arduino.h>

#define BUZZER_PIN 33

const int paddleBounceTone = 500; 
const int paddleBounceDuration = 50;

const int smashTone = 800;      
const int smashDuration = 100;  

void playPaddleBounce() {
  tone(BUZZER_PIN, paddleBounceTone, paddleBounceDuration);
  delay(paddleBounceDuration);
  noTone(BUZZER_PIN);
}

void playSmashSound() {
  tone(BUZZER_PIN, smashTone, smashDuration);
  delay(smashDuration);
  noTone(BUZZER_PIN);
}

#endif