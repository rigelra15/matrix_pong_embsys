#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <Arduino.h>
#define BUZZER_PIN 33

const byte number3Module2[8] = {
  0b00000111,
  0b00000000,
  0b00000000,
  0b00000111,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000111,
};

const byte number3Module1[8] = {
  0b11100000,
  0b00100000,
  0b00100000,
  0b11100000,
  0b00100000,
  0b00100000,
  0b00100000,
  0b11100000,
};

const byte number2Module2[8] = {
  0b00000111,
  0b00000000,
  0b00000000,
  0b00000111,
  0b00000100,
  0b00000100,
  0b00000100,
  0b00000111,
};

const byte number2Module1[8] = {
  0b11100000,
  0b00100000,
  0b00100000,
  0b11100000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b11100000,
};

const byte number1Module2[8] = {
  0b00000001,
  0b00000010,
  0b00000100,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000111,
};

const byte number1Module1[8] = {
  0b11000000,
  0b01000000,
  0b01000000,
  0b01000000,
  0b01000000,
  0b01000000,
  0b01000000,
  0b11110000,
};

void playCountdownBeep(bool finalBeep = false) {
  if (finalBeep) {
    tone(BUZZER_PIN, 440, 500);
    delay(600);
  } else {
    tone(BUZZER_PIN, 440, 150);
    delay(200);
  }
  noTone(BUZZER_PIN);
}

#endif