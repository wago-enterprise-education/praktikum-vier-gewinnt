#include <LEDMatrix.h>
#include <Arduino.h>
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
        std::vector<signed char> tmp2;
        for (byte j = 0; j < initNRows; j++) {
            tmp2.push_back(off);
        }
        LEDvalues.push_back(tmp2);
    }

    currentColumn = 0;
}

// Public Methoden:

// Methode zum Setzen des Lichtwerts einer LED
void LEDMatrix::setLightValue(int currentColumnnumber, int previousColumnnumber, byte color) {
    if (currentColumnnumber >= 0 && previousColumnnumber >= 0) {
        LEDvalues[previousColumnnumber][0] = off;
        if (color < 3) { // Lampe soll leuchten und nicht blinken
            std::pair<int, int> tmp = findPossibleDestination(currentColumnnumber);
            rollingStone(currentColumnnumber, tmp.second, color);
        } else { // Lampe soll blinken
            if (flash(1000)) {
                LEDvalues[currentColumnnumber][0] = color;
            } else {
                LEDvalues[currentColumnnumber][0] = off;
            }
        }
    }
}

// Methode zur Aktualisierung des Spiels
bool LEDMatrix::update(bool menu) {
    if(!menu){
        won = (won || winControl());

        if (won) {
            endAnimation();
        } else if (drawControl()) {
            drawAnimation();
        } 
    }
    
    return won;
}

// Methode zum Finden einer freien Spalte
int LEDMatrix::findPossibleDestination(int currentColumnnumber, int direction) {
    int tmp = -1;
    if (currentColumn >= 0) {
        // Schleife, die in einem Kreislauf alle Spalten durchläuft
        for (byte i = (currentColumnnumber + nColumns) % nColumns, j = 0; 
        j < nColumns; i = ((i + direction) + nColumns) % nColumns, j++) {
            if (possibleDestination(i)) {
                return i;
            }
        }
    }
    
    return tmp;
}

// Methode zum Finden einer freien LED
std::pair<int, int> LEDMatrix::findPossibleDestination(int currentColumnnumber) {
    std::pair<int, int> tmp;
    for (byte c = currentColumnnumber; c < nColumns; c++) {
        tmp.first = c;
        for (int r = nRows-1; r >= 0; r--)
        {
            if(LEDvalues[c][r] == off){
                tmp.second = r;
                return tmp;
            }
        }
    }
    tmp.first = -1;
    tmp.second = -1;
    return tmp;
}

// Methode, die alle freien Spalten zurückgibt
std::vector<byte> LEDMatrix::findPossibleColumns(int currentColumnnumber) {
    std::vector<byte> tmp;
    for (byte c = currentColumnnumber; c < nColumns; c++) {
        if(LEDvalues[c][0] == off){
            tmp.push_back(c);
        }
    }
    
    return tmp;
}

// Methode zum Überprüfen, ob ein Ziel möglich ist
bool LEDMatrix::possibleDestination(int currentColumnnumber) {
    return (LEDvalues.at(currentColumnnumber).at(0) == off);
}

// Methode zum Zurücksetzen des Spiels
void LEDMatrix::reset() {
    draw = false;
    resetAnimation();
    // Schaltet alle LEDs aus
    for (byte i = 0; i < nColumns; i++) {
        for (byte j = 0; j < nRows; j++) {
            LEDvalues[i][j] = 0;
        }
    }
    won = false;
}

// Methode zum Setzen der LEDs
void LEDMatrix::setLEDs() {
    // Schaltet alle LEDs aus
    for (byte r = 0; r < nRows; r++) {
        digitalWrite(pins[red][r], LOW);
        digitalWrite(pins[green][r], LOW);
    }

    // Schaltet Masse auf der aktuellen Spalte
    for (byte c = 0; c < nColumns; c++) {
        digitalWrite(pins[0][c], c != currentColumn);
    }

    // Schaltet die LEDs in der Zeile ein
    for (byte row = 0; row < nRows; row++) {
        switch (LEDvalues[currentColumn][row]) {
            case 0: // aus
                digitalWrite(pins[red][row], LOW);
                digitalWrite(pins[green][row], LOW);
                break;

            case 1: // rot
                digitalWrite(pins[red][row], HIGH);
                break;

            case 2: // grün
                digitalWrite(pins[green][row], HIGH);
                break;

            case 3: // blinkend rot
                digitalWrite(pins[red][0], HIGH);
                break;

            case 4: // blinkend grün
                digitalWrite(pins[green][0], HIGH);
                break;
        }
    }
    currentColumn = (currentColumn + 1) % nColumns; // Erhöt die Spaltennummer in einem Kreislauf von 0 bis Anzahl Spalten -1
}

// Methode, die die längsten Reihen auf dem Spielfeld zurück gibt
std::vector<std::vector<std::pair<int, int> > > LEDMatrix::getBestPath(){
    std::vector< std::vector< std::pair<int, int>>> bestPaths;
    std::vector<std::pair<int, int> > path1;
    std::vector<std::pair<int, int> > path2;
    std::vector<std::pair<int, int> > path3;
    std::vector<std::pair<int, int> > path4;
    
    path1 = winControlColumn();
    path2 = winControlDiagonalUpwards();
    path3 = winControlDiagonalDownwards();
    path4 = winControlRow();
    
    bestPaths.push_back(path1);
    bestPaths.push_back(path2);
    bestPaths.push_back(path3);
    bestPaths.push_back(path4);
    
    // Bubble-Sort zum sortieren der Reihen
    int limit = bestPaths.size();
    bool getauscht = false;
    do{
        getauscht = false;
        for(int i = 0; i < limit-1; i++){
            std::vector<std::pair<int, int> > path11 = bestPaths[i];
            std::vector<std::pair<int, int> > path22 = bestPaths[i+1];
            if(path11.size() < path22.size()){
                std::vector<std::pair<int, int> > tmp = path11;
                bestPaths[i] = bestPaths[i+1];
                bestPaths[i+1] = tmp;
                getauscht = true;
            }
        }
        limit--;
    }while(getauscht);

    return bestPaths;
}

// Methode zum Zeichnen einer Zahl auf der LED-Matrix
void LEDMatrix::printNumber(byte number){
    for (byte i = 0; i < nColumns; i++) {
        for (byte j = 0; j < nRows; j++) {
            LEDvalues[i][j] = 0;
        }
    }

    switch (number)
    {
    case 1:
        LEDvalues = {   {0, 0, 0, 0, 0}, 
                        {0, 0, 0, 0, 0}, 
                        {0, 2, 0, 0, 0}, 
                        {2, 2, 2, 2, 2}, 
                        {0, 0, 0, 0, 0}, 
                        {0, 0, 1, 0, 0} };
        break;
    
    case 2:
        LEDvalues = {   {0, 0, 1, 0, 0}, 
                        {0, 0, 0, 0, 0}, 
                        {2, 0, 2, 2, 2}, 
                        {2, 2, 2, 0, 2}, 
                        {0, 0, 0, 0, 0}, 
                        {0, 0, 0, 0, 0} };
        break;
    }
}

// Private Methoden:

// Methode zur Überprüfung des Gewinns
bool LEDMatrix::winControl() {
    // Spalten überprüfen
    if(winControlColumn().size() == 5){
        return HIGH;
    }

    // Zeilen überprüfen
    if(winControlRow().size() == 5){
        return HIGH;
    }

    // Diagonalen oben überprüfen
    if(winControlDiagonalUpwards().size() == 5){
        return HIGH;
    }

    // Diagonalen unten überprüfen
    if(winControlDiagonalDownwards().size() == 5){
        return HIGH;
    }

    return LOW; // Keinen Sieg gefunden
}

// Methode zur Überprüfung des Gewinns in den Reihen
std::vector<std::pair<int, int>> LEDMatrix::winControlRow() {
    std::vector<std::pair<int, int>> bestPath;
    winPath.clear();
    for (size_t r = 0; r < nRows; r++) {
        winPath.clear();
        int lastColor = off;
        int count = 0;
        for (size_t c = 0; c < nColumns; c++) {
            std::pair<int, int> coordinate;
            coordinate.first = c;
            coordinate.second = r;
            int color = LEDvalues[c][r];

            std::pair<int, int> winColor;
            winColor.first = color;
            winColor.second = color;

            if (color == off) {
                lastColor = color;
                count = 0;
                if(winPath.size() > bestPath.size()){
                    bestPath = winPath;
                }
                winPath.clear();
            } else if (lastColor == off) {
                lastColor = color;
                count = 1;
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            } else if (lastColor == color) {
                count++;
                lastColor = color;
                winPath.push_back(coordinate);
            } else {
                count = 1;
                lastColor = color;
                if(winPath.size() > bestPath.size()){
                    bestPath = winPath;
                }
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            }
            if (count == 4) {
                return winPath;
            }
        }
        if(winPath.size() > bestPath.size()){
            bestPath = winPath;
        }
    }
    return bestPath;
}

// Methode zur Überprüfung des Gewinns in den Spalten
std::vector<std::pair<int, int>> LEDMatrix::winControlColumn() {
    std::vector<std::pair<int, int>> bestPath;
    winPath.clear();
    for (size_t c = 0; c < nColumns; c++) {
        winPath.clear();
        int lastColor = off;
        int count = 0;
        for (size_t r = 0; r < nRows; r++) {
            std::pair<int, int> coordinate;
            coordinate.first = c;
            coordinate.second = r;
            int color = LEDvalues[c][r];

            std::pair<int, int> winColor;
            winColor.first = color;
            winColor.second = color;

            if (color == off) {
                lastColor = color;
                count = 0;
                if(winPath.size() > bestPath.size()){
                    bestPath = winPath;
                }
                winPath.clear();
            } else if (lastColor == off) {
                lastColor = color;
                count = 1;
                if(winPath.size() > bestPath.size()){
                    bestPath = winPath;
                }
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            } else if (lastColor == color) {
                count++;
                lastColor = color;
                winPath.push_back(coordinate);
            } else {
                count = 1;
                lastColor = color;
                if(winPath.size() > bestPath.size()){
                    bestPath = winPath;
                }
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            }

            if (count == 4) {
                return winPath;
            }
        }
        if(winPath.size() > bestPath.size()){
            bestPath = winPath;
        }
    }
    return bestPath;
}

// Methode zur Überprüfung des Gewinns in den Diagonalen nach oben
std::vector<std::pair<int, int>> LEDMatrix::winControlDiagonalDownwards() {
    std::vector<std::pair<int, int>> bestPath;
    winPath.clear();
    for (size_t xStart = 0, yStart = 1; xStart < 3;) {
        int lastColor = off;
        int count = 1;
        for (size_t c = xStart, r = yStart; c < nColumns && r < nRows; c++, r++) {
            std::pair<int, int> coordinate;
            coordinate.first = c;
            coordinate.second = r;
            int color = LEDvalues[c][r];

            std::pair<int, int> winColor;
            winColor.first = color;
            winColor.second = color;

            if (color == off) {
                lastColor = color;
                count = 0;
                if(winPath.size() > bestPath.size()){
                    bestPath = winPath;
                }
                winPath.clear();
            } else if (lastColor == off) {
                lastColor = color;
                count = 1;
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            } else if (lastColor == color) {
                count++;
                lastColor = color;
                winPath.push_back(coordinate);
            } else {
                count = 1;
                lastColor = color;
                if(winPath.size() > bestPath.size()){
                    bestPath = winPath;
                }
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            }

            if (count == 4) {
                return winPath;
            }
        }
        if (yStart > 0) {
            yStart--;
        } else {
            xStart++;
        }
        if(winPath.size() > bestPath.size()){
            bestPath = winPath;
        }
    }
    return bestPath;
}

// Methode zur Überprüfung des Gewinns in den Diagonalen nach unten
std::vector<std::pair<int, int>> LEDMatrix::winControlDiagonalUpwards() {
    std::vector<std::pair<int, int>> bestPath;winPath.clear();
    for (size_t xStart = 0, yStart = 3; xStart < 3;) {
        int lastColor = off;
        int count = 1;
        for (size_t c = xStart, r = yStart; c < nColumns && r >= 0; c++, r--) {
            std::pair<int, int> coordinate;
            coordinate.first = c;
            coordinate.second = r;
            int color = LEDvalues[c][r];

            std::pair<int, int> winColor;
            winColor.first = color;
            winColor.second = color;

            if (color == off) {
                lastColor = color;
                count = 0;
                if(winPath.size() > bestPath.size()){
                    bestPath = winPath;
                }
                winPath.clear();
            } else if (lastColor == off) {
                lastColor = color;
                count = 1;
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            } else if (lastColor == color) {
                count++;
                lastColor = color;
                winPath.push_back(coordinate);
            } else {
                count = 1;
                lastColor = color;
                if(winPath.size() > bestPath.size()){
                    bestPath = winPath;
                }
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            }
            if (count == 4) {
                return winPath;
            }
        }
        if (yStart < 4) {
            yStart++;
        } else {
            xStart++;
        }
        if(winPath.size() > bestPath.size()){
            bestPath = winPath;
        }
    }
    return bestPath;
}

// Methode zur Überprüfung von unentschieden 
bool LEDMatrix::drawControl(){
    for (byte r = 0; r < nRows; r++) {
        for (byte c = 0; c < nColumns; c++) {
            if(LEDvalues[c][r] == off || LEDvalues[c][r] > 2){
                return false;
            }
        }
    }
    draw = true;
    return true;
}

// Methode für die Endanimation
void LEDMatrix::endAnimation() {
    if (flash(1000)) {
        for(byte i = 1; i < 5; i++){ // Die 5 steht für die 4 Steine des Gewinns + die Farbe
            LEDvalues[winPath[i].first][winPath[i].second] = winPath[0].first;
        }
    } else {
        for(byte i = 1; i < 5; i++){ // Die 5 steht für die 4 Steine des Gewinns + die Farbe
            LEDvalues[winPath[i].first][winPath[i].second] = off;
        }
    }
}

// Methode für die Animation bei Unentschieden
void LEDMatrix::drawAnimation() {
    if (flash(1000)) {
        setLEDs();
    } else {
        for (int c = 0; c < nColumns; c++) {
            digitalWrite(pins[0][c], HIGH);
        }

        for (int r = 0; r < nRows; r++) {
            digitalWrite(pins[red][r], LOW);
            digitalWrite(pins[green][r], LOW);
        }
    }
}

// Methode für die Animatiom beim Reseten 
void LEDMatrix::resetAnimation(){
    // Kopiert die LEDValues 
    std::vector<std::vector<int> > LEDValuesClone;
    for(byte c = 0; c < nColumns; c++){
        std::vector<int> tmp;
        for(byte r = 0; r < nRows; r++){
            tmp.push_back(LEDvalues[c][r]);
        }
        LEDValuesClone.push_back(tmp);
    }

    bool flanke = false;
    for (byte i = 1; i <= nRows;)
    {
        bool test = flash(200);
        if(test && !flanke){
            flanke = true;
            for (byte r = 0; r < nRows; r++)
            {
                for (byte c = 0; c < nColumns; c++)
                {
                    // Setzt jedes Licht der Zeile eine Zeile nach unten
                    if(r - i >= 0){
                          
                        LEDvalues[c][r] = LEDValuesClone[c][r-i];
                    }
                    else {
                        LEDvalues[c][r] = off;
                    }
                }    
            }
            i++;
        }
        else if(!test && flanke){
            flanke = false;
        }
    }
}

// Methode für das Herunterfallen des Spielsteins
void LEDMatrix::rollingStone(int currentColumnnumber, int currentrownumber, byte color) {
    bool flanke = false;
    for (int i = 0; i <= currentrownumber;) {
        if (flash(200) && !flanke) {
            flanke = true;
            LEDvalues[currentColumnnumber][i] = color;
            if (i > 0) {
                LEDvalues[currentColumnnumber][i - 1] = off;
            }
            i++;
        } else if (!flash(200) && flanke) {
            flanke = false;
        }
    }
}

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

// Methode, die zurückgibt, ob es unentschieden ist
bool LEDMatrix::isDraw(){
    return draw;
}