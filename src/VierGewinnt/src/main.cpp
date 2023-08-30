#include <Arduino.h>
#include <LEDMatrix.h>

#define LED1 18
#define LED2 5
#define LED3 17
#define LED4 16
#define LED5 4
#define LED1Ground 23
#define LED2Ground 22
#define LED3Ground 1
#define LED4Ground 3
#define LED5Ground 21
#define LED6Ground 19

#define tasterL 34
#define tasterR 33
#define tatserRst 35
#define tasterU 32

const int cColumns = 6;
const int cLines = 5;
const int off = 0;
const int red = 1;
const int green = 2;
const int startColor = red;

bool player1 = HIGH;
int currentColor = 2;
int currentColumn = 0;
int LEDMatrixPins[2][cColumns] = {{LED1Ground, LED2Ground, LED3Ground, LED4Ground, LED5Ground, LED6Ground},
                                    {LED1, LED2, LED3, LED4, LED5, 0}};
LEDMatrix lm(&LEDMatrixPins[0][0], cColumns, cLines);

void controlLEDMatrix(byte[2][2] , byte[2][2] );

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