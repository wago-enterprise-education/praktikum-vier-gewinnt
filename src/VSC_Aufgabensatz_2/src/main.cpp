#include <Arduino.h>
#include <LEDMatrix.h>
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
byte currentColumn = 0;
byte lastButton = 0;

// Array für die Pin-Belegung der LED-Matrix
byte LEDMatrixPins[nColumns * 3] = {ground1, ground2, ground3, ground4, ground5, ground6,
                                    line1red, line2red, line3red, line4red, line5red, 0,
                                    line1green, line2green, line3green, line4green, line5green, 0};

// Objekt der LED Matrix
LEDMatrix lm(LEDMatrixPins, nColumns, nRows);

// Task für die Verlagerung auf einen zweiten Prozessor Kern
TaskHandle_t Task1;

// Methode zum updaten der Anzeige
void updateLEDMatrix(void * parameteter);

// Methode zum Auslesen und Verarbeiten der Tastereingaben 
void readButtons();

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
  lm.printNumber(1);
  delay(1);
}

// Methode zum updaten der Anzeige
void updateLEDMatrix(void * parameter){
    (void)parameter;
    for(;;){
        lm.setLEDs();
        delay(1);
    }
}

void readButtons(){
    
}