#include <Arduino.h>
#include <LEDMatrix.h>
#include <miniMax.h>

// Pin-Definitionen für die LEDs und Tasten
#define line1red 21
#define line2red 15  //3
#define line3red 2   //1
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

#define buttonL 34
#define buttonR 33
#define buttonRst 35
#define buttonD 32

// Konstanten für die LED-Matrix
const int nColumns = 6;
const int nRows = 5;
const int off = 0;
const int red = 1;
const int green = 2;
const int flashRed = 3;
const int flashGreen = 4;
const int startColor = red;

// Variablen für den Spielstatus
int lastButton = 0;
bool player1 = HIGH;
bool menu = HIGH;
int currentNumber = 1;
bool multiplayer = false;
int currentColor = startColor;
int currentColumn = 0;
int countPlays = 0;

// Array für die Pin-Belegung der LED-Matrix
int LEDMatrixPins[nColumns * 3] = {ground1, ground2, ground3, ground4, ground5, ground6,
                                    line1red, line2red, line3red, line4red, line5red, 0,
                                    line1green, line2green, line3green, line4green, line5green, 0};
LEDMatrix lm(LEDMatrixPins, nColumns, nRows);
miniMax mm();

// Methode zum Auslesen und Verarbeiten der Tastereingaben 
void readButtons(bool);

// Methode zum Auslesen und Verarbeiten der Tastereingaben im Menu
void readButtons();

// Methode zum Zurücksetzen des Spiels
void reset();

// Methode zum Setzen eines Steins durch die AI
void AImove();

void setup() {
    // Konfiguration der Pins
    pinMode(ground1, OUTPUT);
    pinMode(ground2, OUTPUT);
    pinMode(ground3, OUTPUT);
    pinMode(ground4, OUTPUT);
    pinMode(ground5, OUTPUT);
    pinMode(ground6, OUTPUT);

    pinMode(line1red, OUTPUT);
    pinMode(line2red, OUTPUT);
    pinMode(line3red, OUTPUT);
    pinMode(line4red, OUTPUT);
    pinMode(line5red, OUTPUT);

    pinMode(line1green, OUTPUT);
    pinMode(line2green, OUTPUT);
    pinMode(line3green, OUTPUT);
    pinMode(line4green, OUTPUT);
    pinMode(line5green, OUTPUT);

    pinMode(buttonL, INPUT);
    pinMode(buttonR, INPUT);
    pinMode(buttonD, INPUT);
    pinMode(buttonRst, INPUT);

    // Serielle Kommunikation starten
    Serial.begin(9600);
}

void loop() {
    if(menu){
        lm.update(menu);
        lm.printNumber(currentNumber);
        readButtons();
    }
    else{
        if(multiplayer){
            bool won = lm.update(menu);
            readButtons(won);
        }
        else{
            // Tasten auslesen und Spielstatus aktualisieren
            bool won = lm.update(menu);
            if(!player1 && !won){
                AImove();
            }

            if(player1 || won){
                readButtons(won);
            }
        }
    }
    
    delay(1);
}

// Methode zum Auslesen und Verarbeiten der Tastereingaben 
void readButtons(bool won) {
    if (digitalRead(buttonL)) {
        if (lastButton != buttonL && currentColumn != -1 && !won) {
            int pos = lm.findPossibleDestination(currentColumn-1, -1);
            if (pos >= 0) {
                lastButton = buttonL;
                if (currentColor == red) {
                    lm.setLightValue(pos, currentColumn, flashRed);
                } else {
                    lm.setLightValue(pos, currentColumn, flashGreen);
                }
                currentColumn = pos;
            }
        }
    } else if (digitalRead(buttonR)) {
        if (lastButton != buttonR && currentColumn != -1 && !won) {
            int pos = lm.findPossibleDestination(currentColumn+1, 1);
            if (pos >= 0) {
                lastButton = buttonR;
                if (currentColor == red) {
                    lm.setLightValue(pos, currentColumn, flashRed);
                } else {
                    lm.setLightValue(pos, currentColumn, flashGreen);
                }
                currentColumn = pos;
            }
        }
    } else if (digitalRead(buttonD)) {
        if (lastButton != buttonD && currentColumn != -1 && !won) {
            lastButton = buttonD;
            lm.setLightValue(currentColumn, currentColumn, currentColor);
            player1 = !player1;
            if (player1) {
                currentColor = startColor;
            } else {
                currentColor = green;
            }
            currentColumn = lm.findPossibleDestination(0, 1);
            countPlays++;
        }
    } else if (digitalRead(buttonRst)) {
        if(lastButton != buttonRst){
            lastButton = buttonRst;
            reset();
        }
    } else if (!won) {
        lastButton = 0;
        if (currentColor == red) {
            lm.setLightValue(currentColumn, currentColumn, flashRed);
        } else {
            lm.setLightValue(currentColumn, currentColumn, flashGreen);
        }
    }
}

// Methode zum Auslesen und Verarbeiten der Tastereingaben im Menu
void readButtons(){
    // Tastenabfrage und Aktionen je nach gedrückter Taste
    if (digitalRead(buttonL)) {
        currentNumber = 1;
        multiplayer = false;
    } else if (digitalRead(buttonR)) {
        currentNumber = 2;
        multiplayer = true;
    } else if (digitalRead(buttonD)) {
        menu = false;
        lm.reset();
    } else if (digitalRead(buttonRst)) {
        
    } 
}

// Methode zum Zurücksetzen des Spiels
void reset() {
    // Spiel zurücksetzen
    lm.reset();
    player1 = HIGH;
    currentColor = startColor;
    currentColumn = 0;
    menu = HIGH;
}

// Methode zum Setzen eines Steins durch die AI
void AImove(){
    std::vector<std::vector<std::pair<int, int>>> bestPaths = lm.getBestPath();

    for (int i = 0; i < bestPaths.size(); i++){
        std::vector<std::pair<int, int>> bestPath = bestPaths.at(i);
        if(bestPath.size() >= 3){
            if(bestPath[1].first < bestPath[2].first){
                //Serial.println("Diagonale oben:");
                if(bestPath[1].second > bestPath[2].second){
                    if(bestPath[1].first > 0 && bestPath[1].second < nRows-1){
                        //Serial.println("Diagonale oben links:");
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[1].first-1);
                        if(pos.first == bestPath[1].first-1 && pos.second == bestPath[1].second+1){
                            lm.setLightValue(pos.first, pos.second, currentColor);
                            player1 = !player1;
                            if (player1) {
                                currentColor = startColor;
                            } else {
                                currentColor = green;
                            }
                            currentColumn = lm.findPossibleDestination(0, 1);
                            countPlays++;
                            return;
                        }
                    }
                    if(bestPath[1].first < nColumns-1 && bestPath[1].second > 0){
                        //Serial.println("Diagonale oben rechts:");
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[bestPath.size()-1].first+1);
                        if(pos.first == bestPath[bestPath.size()-1].first+1 && pos.second == bestPath[bestPath.size()-1].second-1){
                            lm.setLightValue(pos.first, pos.second, currentColor);
                            player1 = !player1;
                            if (player1) {
                                currentColor = startColor;
                            } else {
                                currentColor = green;
                            }
                            currentColumn = lm.findPossibleDestination(0, 1);
                            countPlays++;
                            return;
                        }
                    }
                }
                else if(bestPath[1].second < bestPath[2].second){
                    //Serial.println("Diagonale unten:");
                    if(bestPath[1].first > 0 && bestPath[1].second > 0){
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[1].first-1);
                        if(pos.first == bestPath[1].first-1 && pos.second == bestPath[1].second-1){
                            //Serial.println("Diagonale unten links:");
                            lm.setLightValue(pos.first, pos.second, currentColor);
                            player1 = !player1;
                            if (player1) {
                                currentColor = startColor;
                            } else {
                                currentColor = green;
                            }
                            currentColumn = lm.findPossibleDestination(0, 1);
                            countPlays++;
                            return;
                        }
                    }
                    if(bestPath[1].first < nColumns-1 && bestPath[1].second < nRows-1){
                        //Serial.println("Diagonale unten Rechts:");
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[bestPath.size()-1].first+1);
                        if(pos.first == bestPath[bestPath.size()-1].first+1 && pos.second == bestPath[bestPath.size()-1].second+1){
                            lm.setLightValue(pos.first, pos.second, currentColor);
                            player1 = !player1;
                            if (player1) {
                                currentColor = startColor;
                            } else {
                                currentColor = green;
                            }
                            currentColumn = lm.findPossibleDestination(0, 1);
                            countPlays++;
                            return;
                        }
                    }
                }
                else{
                    //Serial.println("Reihe:");
                    if(bestPath[1].first > 0){
                        //Serial.println("Reihe links:");
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[1].first-1);
                        if(pos.second == bestPath[1].second){
                            lm.setLightValue(pos.first, pos.second, currentColor);
                            player1 = !player1;
                            if (player1) {
                                currentColor = startColor;
                            } else {
                                currentColor = green;
                            }
                            currentColumn = lm.findPossibleDestination(0, 1);
                            countPlays++;
                            return;
                        }
                    }
                    if(bestPath[1].first < nColumns-1){
                        //Serial.println("Reihe rechts:");
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[bestPath.size()-1].first+1);
                        if(pos.second == bestPath[1].second){
                            lm.setLightValue(pos.first, pos.second, currentColor);
                            player1 = !player1;
                            if (player1) {
                                currentColor = startColor;
                            } else {
                                currentColor = green;
                            }
                            currentColumn = lm.findPossibleDestination(0, 1);
                            countPlays++;
                            return;
                        }
                    }
                }
            }
            else{
                //Serial.println("Spalte:");
                std::pair<int, int> pos = lm.findPossibleDestination(bestPath[1].first);
                if(pos.first != -1 && pos.second == bestPath[1].second-1){
                    //Serial.println("Spalte2");
                    lm.setLightValue(pos.first, pos.second, currentColor);
                    player1 = !player1;
                    if (player1) {
                        currentColor = startColor;
                    } else {
                        currentColor = green;
                    }
                    currentColumn = lm.findPossibleDestination(0, 1);
                    countPlays++;
                    return;
                }
            }
        }
    }

    std::vector<int> columns = lm.findPossibleColumns(currentColumn);
    int length = columns.size();
    if(length >= 1){
        srand(millis());
        int rdm = rand() % length;

        std::pair<int, int> pos = lm.findPossibleDestination(columns.at(rdm));
        if(pos.first >= 0){
            lm.setLightValue(pos.first, currentColumn, currentColor);
            player1 = !player1;
            if (player1) {
                currentColor = startColor;
            } else {
                currentColor = green;
            }
            currentColumn = lm.findPossibleDestination(0, 1);
            countPlays++;
        }
    }
}
