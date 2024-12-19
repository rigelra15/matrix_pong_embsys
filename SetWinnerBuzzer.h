#ifndef SET_WINNER_BUZZER_H
#define SET_WINNER_BUZZER_H

#include <Arduino.h>

#define BUZZER_PIN 33 

const int setWinMelody[] = {
  440, 494, 523, 440, 0, 440, 494, 523, 440 
};

const int noteDurationsSetWinner[] = {
  200, 200, 400, 200, 200, 200, 200, 400, 600 
};

void playSetWinMelody() {
  for (int i = 0; i < sizeof(setWinMelody) / sizeof(setWinMelody[0]); i++) {
    if (setWinMelody[i] == 0) {
      delay(noteDurationsSetWinner[i]); 
    } else {
      tone(BUZZER_PIN, setWinMelody[i], noteDurationsSetWinner[i]); 
      delay(noteDurationsSetWinner[i] * 1.3); 
      noTone(BUZZER_PIN); 
    }
  }
}

#endif
