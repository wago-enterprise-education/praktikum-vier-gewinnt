#include <Arduino.h>
#include <LEDMatrix.h>

#define line1red 21
#define line2red 3
#define line3red 1
#define line4red 22
#define line5red 23

#define line1green 25
#define line2green 26
#define line3green 27
#define line4green 14
#define line5green 12

#define ground1 19
#define ground2 18
#define ground3 5
#define ground4 17
#define ground5 16
#define ground6 4

#define tasterL 34
#define tasterR 33
#define tatserRst 35
#define tasterU 32

const int cColumns = 6;
const int cLines = 5;
const int off = 0;
const int red = 1;
const int green = 2;
const int flahRed = 3;
const int flashGreen = 4;
const int startColor = red;

bool player1 = HIGH;
int currentColor = 2;
int currentColumn = 0;
int LEDMatrixPins[3][cColumns] = {{ground1, ground2, ground3, ground4, ground5, ground6},
                                    {line1red, line2red, line3red, line4red, line5red, 0},
                                    {line1green, line2green, line3green, line4green, line5green, 0}};
LEDMatrix lm(&LEDMatrixPins[0][0], cColumns, cLines);


void setup() {
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 6; j++){
      if(i == 1 && j == 5) break;
      
      pinMode(LEDMatrixPins[j][i], OUTPUT);
    }
  }

  pinMode(tasterL, INPUT);
  Serial.begin(9600);
}

void loop() {
  updateColumn();
  lm.flashLigth(currentColumn, currentColor);
  if(lm.update()){
    reset();
  }
  delay(1);
}

void updateColumn(){
  if(digitalRead(tasterL)){
    if(currentColumn != 0)
    {
      currentColumn = (currentColumn - 1);
    }
  }
  else if(digitalRead(tasterR)){
    if(currentColumn != cColumns-1)
    {
      currentColumn = (currentColumn + 1);
    }
  }
  else if(digitalRead(tasterU)){
    lm.setNewLigth(currentColumn, currentColor);
    player1 = !player1;
    if(player1){
      currentColor = startColor;
    }
    else{
      currentColor = green;
    }
  }
  else if(digitalRead(tatserRst)){
    reset();
  }
}

void reset(){
  lm.reset();
  player1 = HIGH;
  currentColor = startColor;
  currentColumn = 0;
}