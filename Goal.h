#ifndef GOAL_H
#define GOAL_H

#include <Arduino.h>

#define BUZZER_PIN 33

const int goalMelody[] = {
  440, 494, 523, 440
};

const int goalDurations[] = {
  100, 100, 200, 400
};

void playGoalMelody() {
  for (int i = 0; i < sizeof(goalMelody) / sizeof(goalMelody[0]); i++) {
    tone(BUZZER_PIN, goalMelody[i], goalDurations[i]);
    delay(goalDurations[i] * 1.3);                    
    noTone(BUZZER_PIN);                               
  }
}

#endif