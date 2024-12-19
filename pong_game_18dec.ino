#include <Arduino.h>
#include <LedControl.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <BluetoothSerial.h>
#include "BluetoothBuzzer.h"
#include "Countdown.h"
#include "Goal.h"
#include "Numbers.h"
#include "PaddleSwitchBuzzer.h"
#include "PongGameBuzzer.h"
#include "SwitchProgramBuzzer.h"
#include "SetWinnerBuzzer.h"
#include "WinnerBuzzer.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define LED_PIN    2
#define DATA_PIN   23
#define CLOCK_PIN  18
#define CS_PIN    5
#define NUM_MODULES 4
#define POT_PIN_PLAYER1 25
#define POT_PIN_PLAYER2 4
#define SWITCH_PIN_PLAYER1 32
#define SWITCH_PIN_PLAYER2 19

BluetoothSerial SerialBT;
MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLOCK_PIN, CS_PIN, NUM_MODULES);
LedControl lc = LedControl(DATA_PIN, CLOCK_PIN, CS_PIN, NUM_MODULES);

bool isConnected = false;
String displayText = "Rigel Ramadhani Waloni - 5024221058";
unsigned long previousMillis = 0;
const long ledInterval = 500;    
int scrollSpeed = 55;

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
const int ballSpeedDelayNormal = 110; 
int ballSpeedDelay = ballSpeedDelayNormal;

const int maxSetWins = 3;

bool switchPlayer1 = false;
bool switchPlayer2 = false;

int lastScorer = -1;

unsigned long lastSmashPlayer1 = 0;
unsigned long lastSmashPlayer2 = 0;
const unsigned long smashCooldown = 10000;

bool pongGameProgram = false;
bool pongGameStart = false;
unsigned long lastSwitchPlayer1 = 0;
unsigned long lastSwitchPlayer2 = 0;
const unsigned long waitChangeProgram = 3000;

void displayNumber(int module, int number, bool player2 = false) {
  lc.clearDisplay(module);
  for (int col = 0; col < 8; col++) {
    byte rotatedData = 0;

    if (player2) {
      for (int row = 0; row < 8; row++) {
        
        if (numbers[number][col] & (1 << (7 - row))) {
          rotatedData |= (1 << row);
        }
      }
      lc.setColumn(module, col, rotatedData);
    } else {
      for (int row = 0; row < 8; row++) {
        
        if (numbers[number][row] & (1 << (7 - col))) {
          rotatedData |= (1 << row);
        }
      }
      lc.setRow(module, col, rotatedData);
    }
  }

  
  for (int i = 0; i < setWinsPlayer1; i++) {
    int y1 = (i == 0) ? 0 : 3; 
    lc.setLed(3, y1, 0, true); 
    lc.setLed(3, y1 + 1, 0, true); 
  }

  
  for (int i = 0; i < setWinsPlayer2; i++) {
    int y2 = (i == 0) ? 0 : 3; 
    lc.setLed(0, y2, 7, true); 
    lc.setLed(0, y2 + 1, 7, true); 
  }

  
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

void resetBall(bool randomStart = false) {
  ballSpeedDelay = ballSpeedDelayNormal; 
  
  ballX = 7;  
  ballY = 3;  
  
  int module = (ballX < 8) ? 2 : 1;
  int xPos = (ballX < 8) ? ballX : ballX - 8;

  for (int i = 0; i < paddleHeight; i++) {
    lc.setLed(2, paddleY_Player1 + i, 0, true); 
    lc.setLed(1, paddleY_Player2 + i, 7, true); 
  }

  lc.setLed(module, ballY, xPos, true); 
  delay(1000);
  
  if (randomStart) {
    
    ballDirX = (random(0, 2) == 0) ? -1 : 1;
    ballDirY = (random(0, 2) == 0) ? -1 : 1;
  } else if (lastScorer != -1) {
    
    ballDirX = (lastScorer == 0) ? 1 : -1;
    ballDirY = 0; 
  }

  
  displayNumber(0, scorePlayer2, true);
  displayNumber(3, scorePlayer1, false);

  delay(1000); 
}

void handleGoal(int scorer) {
  if (scorer == 0) {
    scorePlayer2++;
    displayNumber(0, scorePlayer2, true);
  } else if (scorer == 1) {
    scorePlayer1++;
    displayNumber(3, scorePlayer1, false);
  }

  lastScorer = scorer;
  Serial.println(lastScorer);
  playGoalMelody();
  
  for (int i = 0; i < setWinsPlayer1; i++) {
    int y1 = (i == 0) ? 0 : 3; 
    lc.setLed(3, y1, 0, true); 
    lc.setLed(3, y1 + 1, 0, true); 
  }
  
  for (int i = 0; i < setWinsPlayer2; i++) {
    int y2 = (i == 0) ? 0 : 3; 
    lc.setLed(0, y2, 7, true); 
    lc.setLed(0, y2 + 1, 7, true); 
  }
  
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

  resetBall();
}

void startBall() {
  if ((lastScorer == 0 && switchPlayer1) || (lastScorer == 1 && switchPlayer2)) {
    ballDirY = (random(0, 2) == 0) ? -1 : 1; 
  }
}

void checkSetWinner() {
  
  for (int i = 0; i < setWinsPlayer1; i++) {
    int y1 = (i == 0) ? 0 : 3; 
    lc.setLed(3, y1, 0, true); 
    lc.setLed(3, y1 + 1, 0, true); 
  }
  
  for (int i = 0; i < setWinsPlayer2; i++) {
    int y2 = (i == 0) ? 0 : 3; 
    lc.setLed(0, y2, 7, true); 
    lc.setLed(0, y2 + 1, 7, true); 
  }

  if (scorePlayer1 >= 11 && (scorePlayer1 - scorePlayer2) >= 2) {
    setWinsPlayer1++;
    playSetWinMelody();
    Serial.println("Player 1 Wins the Set!");
    if (setWinsPlayer1 < maxSetWins) {
      resetSet();
    }
  }

  if (scorePlayer2 >= 11 && (scorePlayer2 - scorePlayer1) >= 2) {
    setWinsPlayer2++;
    playSetWinMelody();
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

void blinkScore(int module, int score, bool player2) {
  if (player2) {
    for (int i = 0; i < 5; i++) {
      displayNumber(module, score, true);
      delay(200);
      lc.clearDisplay(module);
      delay(200);
    }
    displayNumber(module, score, true);
  } else {
    for (int i = 0; i < 5; i++) {
      displayNumber(module, score, false);
      delay(200);
      lc.clearDisplay(module);
      delay(200);
    }
    displayNumber(module, score, false);
  }
}

void resetSet() {

  if (scorePlayer1 >= 11 && (scorePlayer1 - scorePlayer2) >= 2) {
    blinkScore(3, scorePlayer1, false);
  } else if (scorePlayer2 >= 11 && (scorePlayer2 - scorePlayer1) >= 2) {
    blinkScore(0, scorePlayer2, true);
  }

  scorePlayer1 = 0;
  scorePlayer2 = 0;
  displayNumber(0, scorePlayer2, true);
  displayNumber(3, scorePlayer1, false);
  
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

void updateDisplayParola() {
  display.displayClear(); 
  display.displayReset(); 
  display.displayText(displayText.c_str(), PA_CENTER, scrollSpeed, 500, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void updateDisplayLC() {
  displayNumber(0, scorePlayer2, true);
  displayNumber(3, scorePlayer1, false);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Game Starting...");

  pinMode(CS_PIN, OUTPUT);     
  digitalWrite(CS_PIN, HIGH);  

  
  activateParola();
  display.begin();
  display.setIntensity(5);              
  display.setTextAlignment(PA_CENTER);  
  deactivateParola();                   

  
  activateLedControl();
  for (int i = 0; i < NUM_MODULES; i++) {
    lc.shutdown(i, false);   
    lc.setIntensity(i, 5);   
    lc.clearDisplay(i);      
  }
  deactivateLedControl();    
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(POT_PIN_PLAYER1, INPUT);
  pinMode(POT_PIN_PLAYER2, INPUT);
  pinMode(SWITCH_PIN_PLAYER1, INPUT_PULLUP);
  pinMode(SWITCH_PIN_PLAYER2, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  analogReadResolution(12); 
  
  activateParola();
  display.displayText(displayText.c_str(), PA_CENTER, scrollSpeed, 500, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  SerialBT.begin("Rigel-Matrix-ESP32");
}

void loop() {
  // SWITCH PROGRAM
  if (digitalRead(SWITCH_PIN_PLAYER1) == 1 && digitalRead(SWITCH_PIN_PLAYER2) == 1) {
    if (millis() - lastSwitchPlayer1 >= waitChangeProgram && millis() - lastSwitchPlayer2 >= waitChangeProgram) {
      pongGameProgram = !pongGameProgram;
      playSwitchProgramSound();
      lastSwitchPlayer1 = millis();
      lastSwitchPlayer2 = millis();

      if (pongGameProgram) {
        SerialBT.end();
      } else {
        SerialBT.begin("Rigel-Matrix-ESP32");
      }

      display.displayClear();
      display.displayReset();

      pinMode(CS_PIN, OUTPUT);
      digitalWrite(CS_PIN, HIGH);
      delay(500);
      digitalWrite(CS_PIN, LOW);
    }
  } else {
    lastSwitchPlayer1 = millis();
    lastSwitchPlayer2 = millis();

  }


  // PROGRAM NAME AND NIM DISPLAY
  if (pongGameProgram == false) {
    if (SerialBT.hasClient()) {
      if (!isConnected) {
        isConnected = true;
        playBluetoothConnectSound();
        digitalWrite(LED_PIN, HIGH);
        displayText = "Terhubung!";
        updateDisplayParola();
      }


      if (SerialBT.available()) {
        String receivedData = SerialBT.readString();
        receivedData.trim();

        Serial.print("Data diterima: ");
        Serial.println(receivedData);

        if (receivedData.startsWith("TEXT:")) {
          displayText = receivedData.substring(5);
          updateDisplayParola();
        } else if (receivedData.startsWith("SPEED:")) {
          scrollSpeed = receivedData.substring(6).toInt();
          Serial.print("Scroll Speed diatur ke: ");
          Serial.println(scrollSpeed);
          
      
          display.setSpeed(scrollSpeed);
        }
      }
    } else {
      if (isConnected) {
        isConnected = false;
        playBluetoothDisconnectSound();
        digitalWrite(LED_PIN, LOW);
        displayText = "Rigel Ramadhani Waloni - 5024221058";
        updateDisplayParola();
      }

      blinkLED();
    }

    if (display.displayAnimate()) {
      display.displayReset();
    }
  }


    // PROGRAM PONG GAME
  if (pongGameProgram) {
    if (pongGameStart == false) {
      
      deactivateParola(); 
      activateLedControl(); 
      
      for (int i = 0; i < NUM_MODULES; i++) {
          lc.shutdown(i, false); 
          lc.setIntensity(i, 5); 
          lc.clearDisplay(i);    
      }

      showPong(lc);
      delay(1000);

      showGame(lc);
      delay(1000);

      showCountdown();
    
      ballX = 7;
      ballY = 3;
    
      int module = (ballX < 8) ? 2 : 1;
      int xPos = (ballX < 8) ? ballX : ballX - 8;
      lc.setLed(module, ballY, xPos, true);
      delay(2000);

      displayNumber(0, scorePlayer2, true);
      displayNumber(3, scorePlayer1, false);

      pongGameStart = true;
  }
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
        handleGoal(0);
      }
      if (ballX > 15) {
        handleGoal(1);
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
}

void activateParola() {
  deactivateLedControl();     
  digitalWrite(CS_PIN, LOW);  
}

void deactivateParola() {
  digitalWrite(CS_PIN, HIGH);  
}

void activateLedControl() {
  deactivateParola();         
  digitalWrite(CS_PIN, LOW);  
}

void deactivateLedControl() {
  digitalWrite(CS_PIN, HIGH);  
}

void blinkLED() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= ledInterval) {
    previousMillis = currentMillis;
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}