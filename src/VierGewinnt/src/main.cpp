#include <Arduino.h>
#include <LEDMatrix.h>

#define line1red 21
#define line2red 15//3
#define line3red 2//1
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

int lastButton = 0;
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
  pinMode(ground1,OUTPUT);
  pinMode(ground2,OUTPUT);
  pinMode(ground3,OUTPUT);
  pinMode(ground4,OUTPUT);
  pinMode(ground5,OUTPUT);
  pinMode(ground6,OUTPUT);
  
  pinMode(line1red, OUTPUT);
  pinMode(line2red, OUTPUT);
  pinMode(line3red, OUTPUT);
  pinMode(line4red, OUTPUT);
  pinMode(line5red, OUTPUT);
  
  pinMode(line1green, OUTPUT);
  pinMode(line2green, OUTPUT);
  pinMode(line3green,OUTPUT);
  pinMode(line4green, OUTPUT);
  pinMode(line5green,OUTPUT);

  pinMode(tasterL, INPUT);
  pinMode(tasterR, INPUT);
  pinMode(tasterU, INPUT);
  pinMode(tasterRst, INPUT);

  Serial.begin(9600);
}

void loop() {
  readButtons();
  
  
  
  if(lm.update()){
    //Serial.println("Gewonnen");
    reset();
  }
  delay(1);
}

void readButtons(){
  if(digitalRead(tasterL)){
    if(currentColumn != 0 && lastButton != tasterL && currentColumn != -1)
    {
      int pos = lm.findPossibleDestination(currentColumn-1, -1);
      if(pos >= 0){
        lastButton = tasterL;
        if(currentColor == red){
          lm.setLightValue(pos, currentColumn, flashRed);
        }
        else{
          lm.setLightValue(pos, currentColumn, flashGreen);
        }
        currentColumn = pos;
      }
    }
  }
  else if(digitalRead(tasterR)){
    
    if(currentColumn != nColumns-1 && lastButton != tasterR && currentColumn != -1)
    {
      int pos = lm.findPossibleDestination(currentColumn+1, 1);
      if(pos >= 0)
      {
        lastButton = tasterR;
        if(currentColor == red){
          lm.setLightValue(pos, currentColumn, flashRed);
        }
        else{
          lm.setLightValue(pos, currentColumn, flashGreen);
        }
        currentColumn = pos;
      }
    }
    
  }
  else if(digitalRead(tasterU)){
    if(lastButton != tasterU && currentColumn != -1)
    {
      lastButton = tasterU;
      lm.dropDown = true;
      //lm.setLightValue(currentColumn, currentColumn, currentColor);
      player1 = !player1;
      if(player1){
        currentColor = startColor;
      }
      else{
        currentColor = green;
      }
      currentColumn = lm.findPossibleDestination(0, 1);
      Serial.println((int)currentColumn);
    }
  }
  else if(digitalRead(tasterRst)){
    reset();
  }
  else{
    lastButton = 0;
    if(currentColor == red){
        lm.setLightValue(currentColumn, currentColumn, flashRed);
      }
      else{
        lm.setLightValue(currentColumn, currentColumn, flashGreen);
      }
  }
}

void reset(){
  lm.reset();
  player1 = HIGH;
  currentColor = startColor;
  currentColumn = 0;
}
