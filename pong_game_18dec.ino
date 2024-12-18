#include <Arduino.h>
#include <LedControl.h>
#include "Numbers.h"
#include "WinnerBuzzer.h"
#include "Countdown.h"
#include "Goal.h"
#include "PaddleSwitchBuzzer.h"
#include "PongGameBuzzer.h"

#define DATA_PIN   23
#define CLOCK_PIN  18
#define LOAD_PIN    5
#define NUM_MODULES 4
#define POT_PIN_PLAYER1 25
#define POT_PIN_PLAYER2 4
#define SWITCH_PIN_PLAYER1 32
#define SWITCH_PIN_PLAYER2 19

LedControl lc = LedControl(DATA_PIN, CLOCK_PIN, LOAD_PIN, NUM_MODULES);

int paddleHeight = 3;
int paddleY_Player1 = 0;
int paddleY_Player2 = 0;
int scorePlayer1 = 0;
int scorePlayer2 = 0;
int lastScorePlayer1 = -1;
int lastScorePlayer2 = -1;
int ballX;
int ballY;
int ballDirX = 1;
int ballDirY = 1;
int setWinsPlayer1 = 0;
int setWinsPlayer2 = 0;

unsigned long lastBallMoveTime = 0;
const int ballSpeedDelayNormal = 120; 
int ballSpeedDelay = ballSpeedDelayNormal;

const int maxSetWins = 3;

bool switchPlayer1 = false;
bool switchPlayer2 = false;

unsigned long lastSmashPlayer1 = 0;
unsigned long lastSmashPlayer2 = 0;
const unsigned long smashCooldown = 10000;

void displayNumber(int module, int number) {
  lc.clearDisplay(module);
  for (int row = 0; row < 8; row++) {
    lc.setRow(module, row, numbers[number][row]);
  }
}

void showCountdown() {

  for (int i = 0; i < NUM_MODULES; i++) {
    lc.clearDisplay(i);
  }


  lc.clearDisplay(1);
  lc.clearDisplay(2);
  for (int i = 0; i < 8; i++) {
    lc.setRow(2, i, number3Module2[i]);
    lc.setRow(1, i, number3Module1[i]);
  }
  playCountdownBeep();
  delay(1000);


  lc.clearDisplay(1);
  lc.clearDisplay(2);
  for (int i = 0; i < 8; i++) {
    lc.setRow(2, i, number2Module2[i]);
    lc.setRow(1, i, number2Module1[i]);
  }
  playCountdownBeep();
  delay(1000);


  lc.clearDisplay(1);
  lc.clearDisplay(2);
  for (int i = 0; i < 8; i++) {
    lc.setRow(2, i, number1Module2[i]);
    lc.setRow(1, i, number1Module1[i]);
  }
  playCountdownBeep();
  delay(1000);


  lc.clearDisplay(1);
  lc.clearDisplay(2);
  playCountdownBeep(true);
}

void updateSmashIndicator() {

  if (millis() - lastSmashPlayer1 >= smashCooldown) {
    lc.setLed(3, 0, 7, true);
  } else {
    lc.setLed(3, 0, 7, false);
  }


  if (millis() - lastSmashPlayer2 >= smashCooldown) {
    lc.setLed(0, 0, 0, true);
  } else {
    lc.setLed(0, 0, 0, false);
  }
}

void resetBall() {
  ballX = random(6, 10);
  ballY = random(1, 6);
  ballDirX = (random(0, 2) == 0) ? -1 : 1;
  ballDirY = (random(0, 2) == 0) ? -1 : 1;
  ballSpeedDelay = ballSpeedDelayNormal;


  displayNumber(0, scorePlayer2);
  displayNumber(3, scorePlayer1);

  delay(2000);
}

void checkSetWinner() {

  if (scorePlayer1 >= 11 && (scorePlayer1 - scorePlayer2) >= 2) {
    setWinsPlayer1++;
    Serial.println("Player 1 Wins the Set!");
    if (setWinsPlayer1 < maxSetWins) {
      resetSet();
    }
  }


  if (scorePlayer2 >= 11 && (scorePlayer2 - scorePlayer1) >= 2) {
    setWinsPlayer2++;
    Serial.println("Player 2 Wins the Set!");
    if (setWinsPlayer2 < maxSetWins) {
      resetSet();
    }
  }


  if (setWinsPlayer1 == maxSetWins) {
    Serial.println("Player 1 Wins the Match!");
    
    lc.clearDisplay(0);
    lc.clearDisplay(1);
    lc.clearDisplay(2);
    lc.clearDisplay(3);

    showWinner(1);  
    delay(50);      
    playWinningMelody();
    while (true) {} 
  }

  if (setWinsPlayer2 == maxSetWins) {
    Serial.println("Player 2 Wins the Match!");
    
    lc.clearDisplay(0);
    lc.clearDisplay(1);
    lc.clearDisplay(2);
    lc.clearDisplay(3);

    showWinner(2);  
    delay(50);      
    playWinningMelody();
    while (true) {} 
  }
}

void blinkScore(int module, int score) {

  for (int i = 0; i < 5; i++) {
    displayNumber(module, score);
    delay(200);
    lc.clearDisplay(module);
    delay(200);
  }
  displayNumber(module, score);
}

void resetSet() {

  if (scorePlayer1 >= 11 && (scorePlayer1 - scorePlayer2) >= 2) {
    blinkScore(3, scorePlayer1);
  } else if (scorePlayer2 >= 11 && (scorePlayer2 - scorePlayer1) >= 2) {
    blinkScore(0, scorePlayer2);
  }


  scorePlayer1 = 0;
  scorePlayer2 = 0;
  displayNumber(0, scorePlayer2);
  displayNumber(3, scorePlayer1);
  
  delay(1000);
  resetBall();
}

void showWinner(int player) {

  for (int i = 0; i < NUM_MODULES; i++) {
    lc.clearDisplay(i);
  }


  for (int i = 0; i < 8; i++) {
    if (player == 1) {
      lc.setRow(3, i, number1[i]);
    } else if (player == 2) {
      lc.setRow(3, i, number2[i]);
    }
  }


  for (int i = 0; i < 8; i++) {
    lc.setRow(2, i, letterW[i]);
  }


  for (int i = 0; i < 8; i++) {
    lc.setRow(1, i, letterI[i]);
  }


  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, letterN[i]);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(POT_PIN_PLAYER1, INPUT);
  pinMode(POT_PIN_PLAYER2, INPUT);
  pinMode(SWITCH_PIN_PLAYER1, INPUT_PULLUP);
  pinMode(SWITCH_PIN_PLAYER2, INPUT_PULLUP);

  for (int i = 0; i < NUM_MODULES; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 5);
    lc.clearDisplay(i);
  }

  analogReadResolution(12);


  showPong(lc);
  delay(1000);


  showGame(lc);
  delay(1000);

  showCountdown();

  displayNumber(0, scorePlayer2);
  displayNumber(3, scorePlayer1);
  
  resetBall();
}

void loop() {
  static int lastBallX = -1, lastBallY = -1;


  paddleY_Player1 = map(analogRead(POT_PIN_PLAYER1), 0, 4095, 0, 5);
  paddleY_Player2 = map(analogRead(POT_PIN_PLAYER2), 0, 4095, 0, 5);
  switchPlayer1 = digitalRead(SWITCH_PIN_PLAYER1);
  switchPlayer2 = digitalRead(SWITCH_PIN_PLAYER2);


  updateSmashIndicator();


  if (millis() - lastBallMoveTime >= ballSpeedDelay) {
    lastBallMoveTime = millis();

  
    if (lastBallX >= 0) {
      int prevModule = (lastBallX < 8) ? 2 : 1;
      int prevXPos = (lastBallX < 8) ? lastBallX : lastBallX - 8;
      lc.setLed(prevModule, lastBallY, prevXPos, false);
    }

  
    ballX += ballDirX;
    ballY += ballDirY;

  
    if (ballY == 0 || ballY == 7) {
      ballDirY = -ballDirY;
    }

  
    if (ballX == 0 && ballY >= paddleY_Player1 && ballY < paddleY_Player1 + paddleHeight) {
      ballDirX = 1; 
      playPaddleBounce();

    
      if (switchPlayer1 == 1 && millis() - lastSmashPlayer1 >= smashCooldown) { 
        ballSpeedDelay = 30; 
        ballDirY = 0;         
        playSmashSound();
        lastSmashPlayer1 = millis();
      } else if (ballDirY == 0) {
        ballDirY = (random(0, 2) == 0) ? -1 : 1; 
        ballSpeedDelay = ballSpeedDelayNormal;   
      }
    }

  
    if (ballX == 15 && ballY >= paddleY_Player2 && ballY < paddleY_Player2 + paddleHeight) {
      ballDirX = -1; 
      playPaddleBounce();

    
      if (switchPlayer2 == 1 && millis() - lastSmashPlayer2 >= smashCooldown) { 
        ballSpeedDelay = 30; 
        ballDirY = 0;         
        playSmashSound();
        lastSmashPlayer2 = millis();
      } else if (ballDirY == 0) {
        ballDirY = (random(0, 2) == 0) ? -1 : 1; 
        ballSpeedDelay = ballSpeedDelayNormal;   
      }
    }

  
    if (ballX < 0) {
      scorePlayer2++;
      playGoalMelody();
      displayNumber(0, scorePlayer2);
      resetBall();
    }
    if (ballX > 15) {
      scorePlayer1++;
      playGoalMelody();
      displayNumber(3, scorePlayer1);
      resetBall();
    }

  
    checkSetWinner();

  
    lastBallX = ballX;
    lastBallY = ballY;
  }


  lc.clearDisplay(2);
  for (int i = 0; i < paddleHeight; i++) {
    lc.setLed(2, paddleY_Player1 + i, 0, true);
  }


  lc.clearDisplay(1);
  for (int i = 0; i < paddleHeight; i++) {
    lc.setLed(1, paddleY_Player2 + i, 7, true);
  }


  int module = (ballX < 8) ? 2 : 1;
  int xPos = (ballX < 8) ? ballX : ballX - 8;
  lc.setLed(module, ballY, xPos, true);

  delay(10);
}