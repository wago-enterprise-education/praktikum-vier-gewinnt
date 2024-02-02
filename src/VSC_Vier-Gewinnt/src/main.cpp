#include <Arduino.h>
#include <LEDMatrix.h>
#include <miniMax.h>
#include <Color.h>


// Pin-Definitionen für die LEDs und Tasten
#define line1red 21
#define line2red 15  
#define line3red 2   
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
const byte nColumns = 6;
const byte nRows = 5;

// Variablen für den Spielstatus
Color startColor = Color::RED;
int currentColumn = 0;
byte lastButton = 0;
Color currentColor = startColor;
byte countPlays = 0;
byte currentMenuNumber = 1;
bool multiplayer = LOW;
bool player1 = HIGH;
bool menu = HIGH;
bool won = LOW;
ulong lastTime;

// Array für die Pin-Belegung der LED-Matrix
byte LEDMatrixPins[nColumns * 3] = {ground1, ground2, ground3, ground4, ground5, ground6,
                                    line1red, line2red, line3red, line4red, line5red, 0,
                                    line1green, line2green, line3green, line4green, line5green, 0};

// Objekt der LED Matrix
LEDMatrix lm(LEDMatrixPins, nColumns, nRows);

// Objekt des MiniMax Algorithmus
miniMax minMax;

// Task für die Verlagerung auf einen zweiten Prozessor Kern
TaskHandle_t Task1;

// Methode zum updaten der Anzeige
void updateLEDMatrix(void * parameteter);

// Methode zum Auslesen und Verarbeiten der Tastereingaben 
void readButtons(bool);

// Methode zum Auslesen und Verarbeiten der Tastereingaben im Menu
void readButtons();

// Methode zum Zurücksetzen des Spiels
void reset();

// Methode zum Setzen eines Steins durch die AI
void AImove();

// Berechnet einen Zug mit dem Minimax und setzt diesen
bool miniMaxMove();

// Methode zum berechnen eines zufälligen Zuges
void randomMove();

// Methode zum Berechnen eines Zuges durch Beachtung von Regeln
bool rulebasedMove();

// Platziert einen Stein und wechselt den Spieler
void placeStone(byte column);

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

    // Erstellen der Task
    xTaskCreatePinnedToCore(
        updateLEDMatrix,    // Methode die verlagert wird
        "Task1",            // Name der Task
        1000,               // Speicher größe
        NULL,
        1,                  // Priorität
        &Task1,             // Task
        0                   // Prozessor Kern
    );

    // Serielle Kommunikation starten
    Serial.begin(9600);
}

// Regelt die Spiellogik
void loop() {
    if(menu){
        won = lm.update(menu);  
        lm.printNumber(currentMenuNumber);  // Bildet den aktuellen Spielmodus ab
        readButtons();
    }
    else{
        if(multiplayer){
            won = lm.update(menu);
            readButtons(won);
        }
        else{
            
            won = lm.update(menu);
            if(!player1 && !won){
                AImove();
            }

            if(player1 || won){
                // Tasten auslesen und Spielstatus aktualisieren
                readButtons(won);
            }
        }
    }
    delay(1);
}

// Methode zum updaten der Anzeige
void updateLEDMatrix(void * parameter){
    (void)parameter;
    for(;;){
        if(!lm.isDraw()){
            lm.setLEDs();
        }
        delay(1);
    }
}

// Methode zum Auslesen und Verarbeiten der Tastereingaben 
void readButtons(bool won) {
    if(digitalRead(buttonRst)){
        if(lastButton == buttonRst && millis() - lastTime >= 1000){
            reset();
        } else if(lastButton != buttonRst){
            lastButton = buttonRst;
            lastTime = millis();
        }
    } else if(!won && currentColumn != -1){
        int pos = currentColumn;
        if(digitalRead(buttonD)){
            if(lastButton != buttonD){
                lastButton = buttonD;
                placeStone(currentColumn);
                return;
            }
        } else if (digitalRead(buttonL)){
            if(lastButton != buttonL){
                lastButton = buttonL;
                pos = lm.findPossibleDestination(currentColumn-1, -1);
            }
        } else if (digitalRead(buttonR)){
            if(lastButton != buttonR){
                lastButton = buttonR;
                pos = lm.findPossibleDestination(currentColumn+1, 1);
            }
        } else{
            lastButton = 0;
        }
        if (pos >= 0) {
            // 
            lm.setLightValue(pos, currentColumn, (Color)((int)(currentColor)+2));
            currentColumn = pos;
        }
    } else{
        lastButton = 0;
    }
}

// Methode zum Auslesen und Verarbeiten der Tastereingaben im Menu
void readButtons(){
    // Tastenabfrage und Aktionen je nach gedrückter Taste
    if (digitalRead(buttonL)) {
        currentMenuNumber = 1;
        multiplayer = false;
    } else if (digitalRead(buttonR)) {
        currentMenuNumber = 2;
        multiplayer = true;
    } else if (digitalRead(buttonD)) {
        menu = false;
        lm.reset(currentColumn);
        player1 = HIGH;
        currentColor = startColor;
        currentColumn = 0;
        countPlays = 0;
    } 
}

// Methode zum Zurücksetzen des Spiels
void reset() {
    // Spiel zurücksetzen
    lm.reset(currentColumn);
    player1 = HIGH;
    currentColor = startColor;
    currentColumn = 0;
    menu = HIGH;
    countPlays = 0;
    lastTime = 0;
    lastButton = 0;
}

// Methode zum Setzen eines Steins durch die AI
void AImove(){
    // if(miniMaxMove()){
    //     Serial.println("MiniMax");
    //     return;
    // }
    if(rulebasedMove()){
        Serial.println("RuleBased");
        return;
    }
    randomMove();
    Serial.println("RandomMove");
}

// Platziert einen Stein und wechselt den Spieler
void placeStone(byte column){
    lm.setLightValue(column, currentColumn, currentColor);
    player1 = !player1;
    if (player1) {
        currentColor = startColor;
    } else {
        currentColor = Color::GREEN;
    }
    currentColumn = lm.findPossibleDestination(0, 1);
    countPlays++;
}

// Methode zum Berechnen eines Zuges durch Beachtung von Regeln
bool rulebasedMove(){
    // Gibt die besten Reihen von rot und grün
    std::vector<std::vector<std::pair<int, int>>> bestPaths = lm.getBestPath();
    for (byte i = 0; i < bestPaths.size(); i++){
        std::vector<std::pair<int, int>> bestPath = bestPaths.at(i);
        if(bestPath.size() >= 2){ // Ist es mindestens eine 2er Reihe?
            if(bestPath[1].first < bestPath[2].first){ 
                if(bestPath[1].second > bestPath[2].second){ // Die Reihe liegt in einer Diagonalen nach oben
                    if(bestPath[1].first > 0 && bestPath[1].second < nRows-1){ // Kontermöglichkeit links von der Reihe
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[1].first-1);
                        if(pos.first == bestPath[1].first-1 && pos.second == bestPath[1].second+1){
                            placeStone(pos.first);
                            return true;
                        }
                    }
                    if(bestPath[1].first < nColumns-1 && bestPath[1].second > 0){ // Kontermöglichkeit rechts von der Reihe
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[bestPath.size()-1].first+1);
                        if(pos.first == bestPath[bestPath.size()-1].first+1 && pos.second == bestPath[bestPath.size()-1].second-1){
                            placeStone(pos.first);
                            return true;
                        }
                    }
                }
                else if(bestPath[1].second < bestPath[2].second){ // Die Reihe liegt in einer Diagonalen nach unten
                    if(bestPath[1].first > 0 && bestPath[1].second > 0){ // Kontermöglichkeit links von der Reihe
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[1].first-1);
                        if(pos.first == bestPath[1].first-1 && pos.second == bestPath[1].second-1){
                            placeStone(pos.first);
                            return true;
                        }
                    }
                    if(bestPath[1].first < nColumns-1 && bestPath[1].second < nRows-1){ // Kontermöglichkeit rechts von der Reihe
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[bestPath.size()-1].first+1);
                        if(pos.first == bestPath[bestPath.size()-1].first+1 && pos.second == bestPath[bestPath.size()-1].second+1){
                            placeStone(pos.first);
                            return true;
                        }
                    }
                }
                else{ // Die Reihe liegt in einer Zeile
                    if(bestPath[1].first > 0){
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[1].first-1);
                        if(pos.second == bestPath[1].second){ // Kontermöglichkeit links von der Reihe
                            placeStone(pos.first);
                            return true;
                        }
                    }
                    if(bestPath[1].first < nColumns-1){
                        std::pair<int, int> pos = lm.findPossibleDestination(bestPath[bestPath.size()-1].first+1);
                        if(pos.second == bestPath[1].second){ // Kontermöglichkeit rechts von der Reihe
                            placeStone(pos.first);
                            return true;
                        }
                    }
                }
            }
            else{ // Die Reihe liegt in einer Spalte
                std::pair<int, int> pos = lm.findPossibleDestination(bestPath[1].first);
                if(pos.first != -1 && pos.second == bestPath[1].second-1){
                    placeStone(pos.first);
                    return true;
                }
            }
        }
    }
    return false; // Es wurde kein Zug gefunden
}

// Methode zum berechnen eines zufälligen Zuges
void randomMove(){
    std::vector<byte> columns = lm.findPossibleColumns(currentColumn);
    byte length = columns.size();
    if(length >= 1){
        srand(millis());
        int rdm = rand() % length;

        std::pair<int, int> pos = lm.findPossibleDestination(columns.at(rdm));
        if(pos.first >= 0){
            placeStone(pos.first);
        }
    }
}

// Berechnet einen Zug mit dem Minimax und setzt diesen
// bool miniMaxMove(){
//     ulong time = millis();
//     byte startDepth = 4;
//     byte startRowToLose = 3;
//     byte standardRowToLose = 4;
//     byte depth = 6;
//     byte toDeepToCalculate = 6;
//     int nothingFound = -1;

//     if(countPlays >= 6){        //Ab 6 Spielzüge soll der Minimax bis eine Spieltiefe von 6 berechnen
//         if(countPlays >= 20){   //Ab 20 Spielzüge soll der Minimax das ganze Spiel vorberechnen
//             depth = 30 - countPlays;
//         }
//         std::pair<int, int> bestPlay = minMax.run(lm.LEDvalues, depth, standardRowToLose);
//         Serial.println("Best move");
//         Serial.print(bestPlay.first);
//         Serial.println(" ");
//         Serial.println(bestPlay.second);
//         if(bestPlay.first >= nothingFound && bestPlay.first != toDeepToCalculate){
//             placeStone(bestPlay.second);
//             float u = (millis()-time) / 1000;
//             Serial.print("Die Berechnung dauerte: ");
//             Serial.print(u);
//             Serial.println(" Sekunden");
//             return true;
//         }
//         else
//         {
//             float u = (millis()-time) / 1000;
//             Serial.print("Die Berechnung dauerte: ");
//             Serial.print(u);
//             Serial.println(" Sekunden");
//             return false;
//         }
//     }
//     else
//     {
//         std::pair<int, int> bestPlay = minMax.run(lm.LEDvalues, startDepth, startRowToLose);
//         Serial.println("Best move");
//         Serial.print(bestPlay.first);
//         Serial.println(" ");
//         Serial.println(bestPlay.second);
//         if(bestPlay.first >= -1 && bestPlay.first != toDeepToCalculate){
//             placeStone(bestPlay.second);
//             float u = (millis()-time) / 1000;
//             Serial.print("Die Berechnung dauerte: ");
//             Serial.print(u);
//             Serial.println(" Sekunden");
//             return true;
//         }
//         else
//         {            
//             float u = (millis()-time) / 1000;
//             Serial.print("Die Berechnung dauerte: ");
//             Serial.print(u);
//             Serial.println(" Sekunden");
//             return false;
//         }
//     }
//     float u = (millis()-time) / 1000;
//     Serial.print("Die Berechnung dauerte: ");
//     Serial.print(u);
//     Serial.println(" Sekunden");
//     return false; // Es wurde kein Zug gefunden
// }