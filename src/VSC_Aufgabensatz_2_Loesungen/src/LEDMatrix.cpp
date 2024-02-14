#include <Arduino.h>
#include <LEDMatrix.h>
#include <vector>
// Konstruktor für die LEDMatrix-Klasse
LEDMatrix::LEDMatrix(byte *p, byte initNColumns, byte initNRows) {
    nColumns = initNColumns;
    nRows = initNRows;

    // Prüfen, ob die Matrix mehr Spalten oder Reihen hat
    int tmp;
    if (initNColumns > initNRows) {
        tmp = initNColumns;
    } else {
        tmp = initNRows;
    }

    // Pin-Belegung übernehmen 
    for (byte i = 0, x = 0; i < 3; i++) {
        std::vector<byte> temp3;
        for (byte j = 0; j < tmp; j++) {
            byte tmp2 = p[x];
            temp3.push_back(tmp2);
            x++;
        }
        pins.push_back(temp3);
    }

    // Initialisierung der LED-Werte auf off
    for (byte i = 0; i < initNColumns; i++) {
        std::vector<Color> tmp2;
        for (byte j = 0; j < initNRows; j++) {
            tmp2.push_back(Color::OFF);
        }
        LEDvalues.push_back(tmp2);
    }

    currentColumn = 0;
}

// Public Methoden:

// Methode zum Setzen des Lichtwerts einer LED
void LEDMatrix::setLightValue(int currentColumnnumber, int previousColumnnumber, Color color) {
    LEDvalues[previousColumnnumber][0] = Color::OFF;
    LEDvalues[currentColumnnumber][0] = color;
}

void LEDMatrix::placeStone(int column, Color color){
    LEDvalues[column][0] = Color::OFF;
    for (byte i = nRows; i >= 0; i--)
    {
        if(LEDvalues[column][i] == Color::OFF){
            LEDvalues[column][i] = color;
            return;
        }
    }
    
}

// Methode zum Setzen der LEDs
void LEDMatrix::setLEDs() {
    // Schaltet alle LEDs aus
    for (byte r = 0; r < nRows; r++) {
        digitalWrite(pins[(int)Color::RED][r], LOW);
        digitalWrite(pins[(int)Color::GREEN][r], LOW);
    }

    // Schaltet Masse auf der aktuellen Spalte
    for (byte c = 0; c < nColumns; c++) {
        digitalWrite(pins[0][c], c != currentColumn);
    }

    // Schaltet die LEDs in der Zeile ein
    for (byte row = 0; row < nRows; row++) {
        switch (LEDvalues[currentColumn][row]) {
            case Color::OFF: // aus
                digitalWrite(pins[(int)Color::RED][row], LOW);
                digitalWrite(pins[(int)Color::GREEN][row], LOW);
                break;

            case Color::RED: // rot
                digitalWrite(pins[(int)Color::RED][row], HIGH);
                break;

            case Color::GREEN: // grün
                digitalWrite(pins[(int)Color::GREEN][row], HIGH);
                break;

            case Color::FLASH_RED: // blinkend rot
                if (flash(1000)) {
                    digitalWrite(pins[(int)Color::RED][0], HIGH);
                }
                break;

            case Color::FLASH_GREEN: // blinkend grün
                if (flash(1000)) {
                    digitalWrite(pins[(int)Color::GREEN][0], HIGH);
                }
                break;
        }
    }
    currentColumn = (currentColumn + 1) % nColumns; // Erhöt die Spaltennummer in einem Kreislauf von 0 bis Anzahl Spalten -1
}

// Methode zum Zeichnen einer Zahl auf der LED-Matrix
void LEDMatrix::printNumber(byte number){
    for (byte i = 0; i < nColumns; i++) {
        for (byte j = 0; j < nRows; j++) {
            LEDvalues[i][j] = Color::OFF;
        }
    }

    switch (number)
    {
    case 0:
        LEDvalues = {   {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF}, 
                        {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF}, 
                        {Color::GREEN, Color::GREEN, Color::GREEN, Color::GREEN, Color::GREEN}, 
                        {Color::GREEN, Color::OFF, Color::OFF, Color::OFF, Color::GREEN}, 
                        {Color::GREEN, Color::GREEN, Color::GREEN, Color::GREEN, Color::GREEN}, 
                        {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF} };
        break;
    case 1:
        LEDvalues = {   {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF}, 
                        {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF}, 
                        {Color::OFF, Color::GREEN, Color::OFF, Color::OFF, Color::OFF},
                        {Color::GREEN, Color::GREEN, Color::GREEN, Color::GREEN, Color::GREEN},  
                        {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF}, 
                        {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF} };
        break;
    
    case 2:
        LEDvalues = {   {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF}, 
                        {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF}, 
                        {Color::GREEN, Color::OFF, Color::GREEN, Color::GREEN, Color::GREEN}, 
                        {Color::GREEN, Color::OFF, Color::GREEN, Color::OFF, Color::GREEN}, 
                        {Color::GREEN, Color::GREEN, Color::GREEN, Color::OFF, Color::GREEN}, 
                        {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF} };
        break;
    case 3:
        LEDvalues = {   {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF}, 
                        {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF}, 
                        {Color::GREEN, Color::OFF, Color::GREEN, Color::OFF, Color::GREEN}, 
                        {Color::GREEN, Color::OFF, Color::GREEN, Color::OFF, Color::GREEN}, 
                        {Color::GREEN, Color::GREEN, Color::GREEN, Color::GREEN, Color::GREEN}, 
                        {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF} };
        break;
    case 4:
        LEDvalues = {   {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF}, 
                        {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF}, 
                        {Color::GREEN, Color::GREEN, Color::GREEN, Color::OFF, Color::OFF}, 
                        {Color::OFF, Color::OFF, Color::GREEN, Color::OFF, Color::OFF},
                        {Color::GREEN, Color::GREEN, Color::GREEN, Color::GREEN, Color::GREEN}, 
                        {Color::OFF, Color::OFF, Color::OFF, Color::OFF, Color::OFF} };
        break;
    }
}

// Private Methoden:

// Methode für das Blinken
bool LEDMatrix::flash(int periodDuration) {
    if (!savedLastTime && lastTime < millis() - periodDuration / 2) {
        lastTime = millis();
        savedLastTime = true;
    }
    switch (step) {
        case 1:
            if (savedLastTime) {
                step = 2;
                savedLastTime = false;
            }
            return true;
            break;

        case 2:
            if (savedLastTime) {
                step = 1;
                savedLastTime = false;
            }
            return false;
            break;
    }
    return false;
}