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
#define tasterRst 35
#define tasterU 32

const int nColumns = 6;
const int nLines = 5;
const int off = 0;
const int red = 1;
const int green = 2;
const int flashRed = 3;
const int flashGreen = 4;
const int startColor = red;

bool player1 = HIGH;
int currentColor = startColor;
int currentColumn = 0;
int LEDMatrixPins[nColumns*3] = {ground1, ground2, ground3, ground4, ground5, ground6,
                                line1red, line2red, line3red, line4red, line5red, 0,
                                line1green, line2green, line3green, line4green, line5green, 0};
LEDMatrix lm(LEDMatrixPins, nColumns, nLines);

void readButtons();
void reset();

void setup() {
  int count = 0;
    for(int i = 0; i < nColumns*3; i++){
      if(i % 6 == 0 && count != 0)
      {

      }
      else
      {
        pinMode(LEDMatrixPins[i], OUTPUT);
      }

      if (i % 6 == 0)
      {
        count++;
      }
      
  }

  pinMode(tasterL, INPUT);
  pinMode(tasterR, INPUT);
  pinMode(tasterU, INPUT);
  pinMode(tasterRst, INPUT);
  Serial.begin(9600);
  pinMode(line1red, OUTPUT);
  pinMode(ground1,OUTPUT);
}

void loop() {
  readButtons();
  digitalWrite(line1red, HIGH);
  digitalWrite(ground1, LOW);
  delay(4000);
  if(currentColor == red){
    lm.setLightValue(currentColumn, flashRed);
  }
  else{
    lm.setLightValue(currentColumn, flashGreen);
  }
  
  if(lm.update()){
    reset();
  }
  delay(1);
}

void readButtons(){
  if(digitalRead(tasterL)){
    if(currentColumn != 0)
    {
      currentColumn = (currentColumn - 1);
    }
  }
  else if(digitalRead(tasterR)){
    if(currentColumn != nColumns-1)
    {
      currentColumn = (currentColumn + 1);
    }
  }
  else if(digitalRead(tasterU)){
    lm.setLightValue(currentColumn, currentColor);
    player1 = !player1;
    if(player1){
      currentColor = startColor;
    }
    else{
      currentColor = green;
    }
  }
  else if(digitalRead(tasterRst)){
    reset();
  }
}

void reset(){
  lm.reset();
  player1 = HIGH;
  currentColor = startColor;
  currentColumn = 0;
}