#include <LEDMatrix.h>
#include <Arduino.h>
#include <vector>

// Konstruktor für die LEDMatrix-Klasse
LEDMatrix::LEDMatrix(int *p, int initNColumns, int initNRows) {
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
    for (int i = 0, x = 0; i < 3; i++) {
        std::vector<int> temp3;
        for (int j = 0; j < tmp; j++) {
            int tmp2 = p[x];
            temp3.push_back(tmp2);
            x++;
        }
        pins.push_back(temp3);
    }

    // Initialisierung der LED-Werte auf off
    for (int i = 0; i < initNColumns; i++) {
        std::vector<int> tmp2;
        for (int j = 0; j < initNRows; j++) {
            tmp2.push_back(off);
        }
        LEDvalues.push_back(tmp2);
    }

    currentColumn = 0;
}

// Methode zum Setzen des Lichtwerts einer LED
void LEDMatrix::setLightValue(int currentColumnnumber, int previousColumnnumber, int color) {
    if (currentColumnnumber >= 0 && previousColumnnumber >= 0) {
        LEDvalues[previousColumnnumber][0] = off;
        if (color < 3) { 
            std::pair<int, int> tmp = findPossibleDestination(currentColumnnumber);
            rollingStone(currentColumnnumber, tmp.second, color);
        } else {
            if (flash(1000)) {
                LEDvalues[currentColumnnumber][0] = color;
            } else {
                LEDvalues[currentColumnnumber][0] = off;
            }
        }
    }
}

// Methode zum Setzen des Lichtwerts einer LED
void LEDMatrix::setLightValue(int column, int row, int previousColumnnumber, int color) {
    LEDvalues[previousColumnnumber][0] = off;
    LEDvalues[column][row] = color;
}

// Methode zur Aktualisierung des Spiels
bool LEDMatrix::update() {
    won = (won || winControl());

    if (won) {
        endAnimation();
    } else if (drawControl()) {
        drawViso();
    } else {
        setLEDs();
    }
    return won;
}

// Methode zum Finden eines möglichen Ziels
int LEDMatrix::findPossibleDestination(int currentColumnnumber, int direction) {
    int tmp = -1;
    if (currentColumn >= 0) {
        for (size_t i = (currentColumnnumber + nColumns) % nColumns, j = 0; 
        j < nColumns; i = ((i + direction) + nColumns) % nColumns, j++) {
            //Serial.println((int)i);
            if (possibleDestination(i)) {
                return i;
            }
        }
    }
    
    return tmp;
}

std::pair<int, int> LEDMatrix::findPossibleDestination(int currentColumnnumber) {
    std::pair<int, int> tmp;
    for (int c = currentColumnnumber; c < nColumns; c++) {
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

std::vector<int> LEDMatrix::findPossibleColumns(int currentColumnnumber) {
    std::vector<int> tmp;
    for (int c = currentColumnnumber; c < nColumns; c++) {
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
    resetAnimation();
    for (int i = 0; i < nColumns; i++) {
        for (int j = 0; j < nRows; j++) {
            LEDvalues[i][j] = 0;
        }
    }
    won = false;
    draw = false;
}

// Methode zum Setzen der LEDs
void LEDMatrix::setLEDs() {
    for (byte r = 0; r < nRows; r++) {
        digitalWrite(pins[red][r], LOW);
        digitalWrite(pins[green][r], LOW);
    }

    for (byte c = 0; c < nColumns; c++) {
        digitalWrite(pins[0][c], c != currentColumn);
    }

    for (int row = 0; row < nRows; row++) {
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
    currentColumn = (currentColumn + 1) % nColumns;
}

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

    return LOW;
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
        for (size_t c = xStart, r = yStart; c < nColumns && r >= 0; c++, r++) {
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

std::vector<std::vector<std::pair<int, int> > > LEDMatrix::getBestPath(){
    std::vector< std::vector< std::pair<int, int>>> bestPaths;
    std::vector<std::pair<int, int> > path1;
    std::vector<std::pair<int, int> > path2;
    std::vector<std::pair<int, int> > path3;
    std::vector<std::pair<int, int> > path4;
    // std::vector<std::pair<int, int> > pathtmp;
    path1 = winControlColumn();
    
    // Serial.println("Spalte:");
    // for (int i = 0; i < path1.size(); i++){

    //     Serial.println((int)path1[i].first);

    //     Serial.println((int)path1[i].second);

    //     Serial.println("----------------");

    // }

    path2 = winControlDiagonalUpwards();

    // Serial.println("Diagonale oben:");
    // for (int i = 0; i < path2.size(); i++){

    //     Serial.println((int)path2[i].first);

    //     Serial.println((int)path2[i].second);

    //     Serial.println("----------------");

    // }

    // if(path2.size() > path1.size()){
    //     pathtmp = path1;
    //     path1 = path2;
    //     path2 = pathtmp;
    // }

    path3 = winControlDiagonalDownwards();

    // Serial.println("Diagonale unten:");
    // for (int i = 0; i < path3.size(); i++){

    //     Serial.println((int)path3[i].first);

    //     Serial.println((int)path3[i].second);

    //     Serial.println("----------------");

    // }

    // if(path3.size() > path1.size()){
    //     pathtmp = path2;
    //     path2 = path3;
    //     path3 = pathtmp;

    //     pathtmp = path1;
    //     path1 = path2;
    //     path2 = pathtmp;
    // }
    // else if(path3.size() > path2.size()){
    //     pathtmp = path2;
    //     path2 = path3;
    //     path3 = pathtmp;
    // }

    path4 = winControlRow();

    // Serial.println("Reihe:");
    // for (int i = 0; i < path4.size(); i++){

    //     Serial.println((int)path4[i].first);

    //     Serial.println((int)path4[i].second);

    //     Serial.println("----------------");

    // }

    // if(path4.size() > path1.size()){
    //     pathtmp = path3;
    //     path3 = path4;
    //     path3 = pathtmp;

    //     pathtmp = path2;
    //     path2 = path3;
    //     path3 = pathtmp;

    //     pathtmp = path1;
    //     path1 = path2;
    //     path2 = pathtmp;
    // }
    // else if(path3.size() > path2.size()){
    //     pathtmp = path3;
    //     path3 = path4;
    //     path3 = pathtmp;

    //     pathtmp = path2;
    //     path2 = path3;
    //     path3 = pathtmp;
    // }
    // else if(path3.size() > path2.size()){
    //     pathtmp = path3;
    //     path3 = path4;
    //     path3 = pathtmp;
    // }

    bestPaths.push_back(path1);
    bestPaths.push_back(path2);
    bestPaths.push_back(path3);
    bestPaths.push_back(path4);
    
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

bool LEDMatrix::comparePaths(std::vector<std::pair<int, int> > path1, std::vector<std::pair<int, int> > path2){
    return (path1.size() > path2.size());
}

// Methode zur Überprüfung von unentschieden 
bool LEDMatrix::drawControl(){
    for (size_t r = 0; r < nRows; r++) {
        for (size_t c = 0; c < nColumns; c++) {
            if(LEDvalues[c][r] == off){
                return false;
            }
        }
    }
    return true;
}

// Methode für die Endanimation
void LEDMatrix::endAnimation() {
    if (flash(1000)) {
        LEDvalues[winPath[1].first][winPath[1].second] = winPath[0].first;
        LEDvalues[winPath[2].first][winPath[2].second] = winPath[0].first;
        LEDvalues[winPath[3].first][winPath[3].second] = winPath[0].first;
        LEDvalues[winPath[4].first][winPath[4].second] = winPath[0].first;
    } else {
        LEDvalues[winPath[1].first][winPath[1].second] = off;
        LEDvalues[winPath[2].first][winPath[2].second] = off;
        LEDvalues[winPath[3].first][winPath[3].second] = off;
        LEDvalues[winPath[4].first][winPath[4].second] = off;
    }
    setLEDs();
}

// Methode für die Animation bei Unentschieden
void LEDMatrix::drawViso() {
    if (flash(1000)) {
        setLEDs();
    } else {
        for (byte c = 0; c < nColumns; c++) {
            digitalWrite(pins[0][c], HIGH);
        }

        for (byte r = 0; r < nRows; r++) {
            digitalWrite(pins[red][r], LOW);
            digitalWrite(pins[green][r], LOW);
        }
    }
}

// Methode für die Animatiom beim Reseten 
void LEDMatrix::resetAnimation(){
    std::vector<std::vector<int> > LEDValuesClone;
    for(int c = 0; c < nColumns; c++){
        std::vector<int> tmp;
        for(int r = 0; r < nRows; r++){
            tmp.push_back(LEDvalues[c][r]);
        }
        LEDValuesClone.push_back(tmp);
    }

    bool flanke = false;
    for (int i = 1; i <= nRows;)
    {
        bool test = flash(200);
        if(test && !flanke){
            flanke = true;
            for (int r = 0; r < nRows; r++)
            {
                for (int c = 0; c < nColumns; c++)
                {
                    if(r - i >= 0){
                          
                        LEDvalues[c][r] = LEDValuesClone[c][r-i];
                    }
                    else {
                        LEDvalues[c][r] = off;
                    }
                    setLEDs();
                    delay(1);
                }    
            }
            setLEDs();
            delay(1);
            i++;
        }
        else if(!test && flanke){
            flanke = false;
        }
        setLEDs();
        delay(1);
    }
}

// Methode für das Herunterfallen des Spielsteins
void LEDMatrix::rollingStone(int currentColumnnumber, int currentrownumber, int color) {
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
        setLEDs();
        delay(1);
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
