#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

#define BUZZER_PIN 33

const int melody[] = {
  440, 494, 523, 440, 440, 494, 523, 440,
  392, 440, 349, 392, 440, 523, 440      
};

const int noteDurations[] = {
  150, 150, 300, 150, 150, 150, 300, 400,
  150, 150, 150, 150, 150, 300, 400      
};

const byte number1[8] = {
  0b00011000,
  0b00111000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00111100,
  0b00000000 
};

const byte number2[8] = {
  0b00111100,
  0b01100110,
  0b00000110,
  0b00011100,
  0b00110000,
  0b01100000,
  0b01111110,
  0b00000000 
};

const byte letterW[8] = {
  0b01000001,
  0b01000001,
  0b01000001,
  0b01001001,
  0b01010101,
  0b01010101,
  0b00100010,
  0b00000000 
};

const byte letterI[8] = {
  0b01111100,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b01111100,
  0b00000000 
};

// const byte letterN[8] = {
//   0b10000010,
//   0b11000010,
//   0b10100010,
//   0b10010010,
//   0b10001010,
//   0b10000110,
//   0b10000010,
//   0b00000000 
// };

void playWinningMelody() {
  for (int repeat = 0; repeat < 3; repeat++) {
    for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
      tone(BUZZER_PIN, melody[i], noteDurations[i]);
      delay(noteDurations[i] * 1.3);                
      noTone(BUZZER_PIN);                           
    }
    delay(500);
  }
}

#endif