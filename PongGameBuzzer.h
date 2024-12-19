#ifndef PONGGAMEBUZZER_H
#define PONGGAMEBUZZER_H

#include <Arduino.h>
#define BUZZER_PIN 33

const byte letterP[8] = {
  0b00111100,
  0b00100100,
  0b00100100,
  0b00111100,
  0b00100000,
  0b00100000,
  0b00100000,
  0b00100000
};

const byte letterO[8] = {
  0b00011100,
  0b00100010,
  0b00100010,
  0b00100010,
  0b00100010,
  0b00100010,
  0b00100010,
  0b00011100
};

const byte letterN[8] = {
  0b00100010,
  0b00110010,
  0b00110010,
  0b00101010,
  0b00101010,
  0b00100110,
  0b00100110,
  0b00100010
};

const byte letterG[8] = {
  0b00011110,
  0b00100000,
  0b00100000,
  0b00100110,
  0b00100010,
  0b00100010,
  0b00100010,
  0b00011110
};

const byte letterA[8] = {
  0b00011100,
  0b00100010,
  0b00100010,
  0b00111110,
  0b00100010,
  0b00100010,
  0b00100010,
  0b00100010
};

const byte letterM[8] = {
  0b00100010,
  0b00110110,
  0b00101010,
  0b00101010,
  0b00101010,
  0b00101010,
  0b00101010,
  0b00100010
};

const byte letterE[8] = {
  0b00111110,
  0b00100000,
  0b00100000,
  0b00111100,
  0b00100000,
  0b00100000,
  0b00100000,
  0b00111110
};

void playLetterBeep() {
  tone(BUZZER_PIN, 440, 100);
  delay(150);
  noTone(BUZZER_PIN);
}

void showPong(LedControl &lc) {
  lc.clearDisplay(3);
  lc.clearDisplay(2);
  lc.clearDisplay(1);
  lc.clearDisplay(0);

  for (int i = 0; i < 8; i++) {
    lc.setRow(3, i, letterP[i]);
  }
  playLetterBeep();

  for (int i = 0; i < 8; i++) {
    lc.setRow(2, i, letterO[i]);
  }
  playLetterBeep();

  for (int i = 0; i < 8; i++) {
    lc.setRow(1, i, letterN[i]);
  }
  playLetterBeep();

  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, letterG[i]);
  }
  playLetterBeep();
}

void showGame(LedControl &lc) {
  lc.clearDisplay(3);
  lc.clearDisplay(2);
  lc.clearDisplay(1);
  lc.clearDisplay(0);

  for (int i = 0; i < 8; i++) {
    lc.setRow(3, i, letterG[i]);
  }
  playLetterBeep();

  for (int i = 0; i < 8; i++) {
    lc.setRow(2, i, letterA[i]);
  }
  playLetterBeep();

  for (int i = 0; i < 8; i++) {
    lc.setRow(1, i, letterM[i]);
  }
  playLetterBeep();

  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, letterE[i]);
  }
  playLetterBeep();
}

#endif