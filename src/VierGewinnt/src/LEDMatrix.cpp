#include <LEDMatrix.h>
#include <Arduino.h>
#include <vector>

// Konstruktor für die LEDMatrix-Klasse
LEDMatrix::LEDMatrix(int *p, int initNColumns, int initNLines) {
    nColumns = initNColumns;
    nLines = initNLines;

    int tmp;
    if (initNColumns > initNLines) {
        tmp = initNColumns;
    } else {
        tmp = initNLines;
    }

    for (int i = 0, x = 0; i < 3; i++) {
        std::vector<int> temp3;
        for (int j = 0; j < tmp; j++) {
            int tmp2 = p[x];
            temp3.push_back(tmp2);
            x++;
        }
        pins.push_back(temp3);
    }

    for (int i = 0; i < initNColumns; i++) {
        std::vector<int> tmp2;
        for (int j = 0; j < initNLines; j++) {
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
            for (int i = nLines - 1; i >= 0; i--) {
                if (LEDvalues.at(currentColumnnumber).at(i) == 0) {
                    rollingStone(currentColumnnumber, i, color);
                    break;
                }
            }
        } else {
            if (flash(1000)) {
                LEDvalues[currentColumnnumber][0] = color;
            } else {
                LEDvalues[currentColumnnumber][0] = off;
            }
        }
    }
}

// Methode zum Überprüfen, ob ein Ziel möglich ist
bool LEDMatrix::possibleDestination(int currentColumnnumber) {
    return (LEDvalues.at(currentColumnnumber).at(0) == off);
}

// Methode zum Finden eines möglichen Ziels
int LEDMatrix::findPossibleDestination(int currentColumnnumber, int direction) {
    int tmp = -1;
    if (currentColumn >= 0) {
        for (size_t i = currentColumnnumber; i < nColumns && i >= 0; i = i + direction) {
            if (possibleDestination(i)) {
                return i;
            }
        }
    }

    draw = (currentColumnnumber == 0 || currentColumnnumber == -1);
    return tmp;
}

// Methode zum Zurücksetzen des Spiels
void LEDMatrix::reset() {
    for (int i = 0; i < nColumns; i++) {
        for (int j = 0; j < nLines; j++) {
            LEDvalues[i][j] = 0;
        }
    }
    won = false;
    draw = false;
}

// Methode zur Aktualisierung des Spiels
bool LEDMatrix::update() {
    won = (won || winControl());

    if (dropDown) {

    }
    if (won) {
        endAnimation();
    } else if (draw) {
        drawViso();
    } else {
        setLEDs();
    }
    return won;
}

// Methode zum Setzen der LEDs
void LEDMatrix::setLEDs() {
    for (byte l = 0; l < nLines; l++) {
        digitalWrite(pins[red][l], LOW);
        digitalWrite(pins[green][l], LOW);
    }

    for (byte c = 0; c < nColumns; c++) {
        digitalWrite(pins[0][c], c != currentColumn);
    }

    for (int line = 0; line < nLines; line++) {
        switch (LEDvalues[currentColumn][line]) {
            case 0: // aus
                digitalWrite(pins[red][line], LOW);
                digitalWrite(pins[green][line], LOW);
                break;

            case 1: // rot
                digitalWrite(pins[red][line], HIGH);
                break;

            case 2: // grün
                digitalWrite(pins[green][line], HIGH);
                break;

            case 3: // blinkend rot
                digitalWrite(pins[red][0], HIGH);
                break;

            case 4: // blinkend grün
                digitalWrite(pins[green][0], HIGH);
                break;
        }
    }
    currentColumn = (currentColumn + 1) % 6;
}

// Methode zur Überprüfung des Gewinns
bool LEDMatrix::winControl() {
    // Spalten überprüfen
    winPath.clear();
    for (size_t c = 0; c < nColumns; c++) {
        int lastColor = off;
        int count = 0;
        for (size_t l = 0; l < nLines; l++) {
            std::pair<int, int> coordinate;
            coordinate.first = c;
            coordinate.second = l;
            int color = LEDvalues[c][l];

            std::pair<int, int> winColor;
            winColor.first = color;
            winColor.second = color;

            if (color == off) {
                lastColor = color;
                count = 0;
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
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            }

            if (count == 4) {
                return HIGH;
            }
        }
        winPath.clear();
    }

    // Zeilen überprüfen
    winPath.clear();
    for (size_t l = 0; l < nLines; l++) {
        int lastColor = off;
        int count = 0;
        for (size_t c = 0; c < nColumns; c++) {
            std::pair<int, int> coordinate;
            coordinate.first = c;
            coordinate.second = l;
            int color = LEDvalues[c][l];

            std::pair<int, int> winColor;
            winColor.first = color;
            winColor.second = color;

            if (color == off) {
                lastColor = color;
                count = 0;
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
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            }

            if (count == 4) {
                return HIGH;
            }
        }
    }

    // Diagonalen oben überprüfen
    winPath.clear();
    for (size_t xStart = 0, yStart = 3; xStart < 3;) {
        int lastColor = off;
        int count = 1;
        for (size_t c = xStart, l = yStart; c < nColumns && l >= 0; c++, l--) {
            std::pair<int, int> coordinate;
            coordinate.first = c;
            coordinate.second = l;
            int color = LEDvalues[c][l];

            std::pair<int, int> winColor;
            winColor.first = color;
            winColor.second = color;

            if (color == off) {
                lastColor = color;
                count = 0;
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
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            }

            if (count == 4) {
                return HIGH;
            }
        }
        winPath.clear();
        if (yStart < 4) {
            yStart++;
        } else {
            xStart++;
        }
    }

    // Diagonalen unten überprüfen
    winPath.clear();
    for (size_t xStart = 0, yStart = 1; xStart < 3;) {
        int lastColor = off;
        int count = 1;
        for (size_t c = xStart, l = yStart; c < nColumns && l >= 0; c++, l++) {
            std::pair<int, int> coordinate;
            coordinate.first = c;
            coordinate.second = l;
            int color = LEDvalues[c][l];

            std::pair<int, int> winColor;
            winColor.first = color;
            winColor.second = color;

            if (color == off) {
                lastColor = color;
                count = 0;
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
                winPath.clear();
                winPath.push_back(winColor);
                winPath.push_back(coordinate);
            }

            if (count == 4) {
                return HIGH;
            }
        }
        winPath.clear();
        if (yStart > 0) {
            yStart--;
        } else {
            xStart++;
        }
    }

    return LOW;
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

// Methode für die Animation bei Unentschieden
void LEDMatrix::drawViso() {
    if (flash(1000)) {
        setLEDs();
    } else {
        for (byte c = 0; c < nColumns; c++) {
            digitalWrite(pins[0][c], HIGH);
        }

        for (byte l = 0; l < nLines; l++) {
            digitalWrite(pins[red][l], LOW);
            digitalWrite(pins[green][l], LOW);
        }
    }
}

// Methode für das Herunterfallen des Spielsteins
void LEDMatrix::rollingStone(int currentColumnnumber, int currentLinenumber, int color) {
    bool flanke = false;
    for (int i = 0; i <= currentLinenumber;) {
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
