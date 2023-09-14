#include <Arduino.h>
#include <vector>
#include "miniMax.h"

// Ist die Hauptmethode. Sie durchläuft alle Wege und gibt den besten zurück
std::pair<int, int> miniMax::run(std::vector<std::vector<signed char>> givenPlayground, int depth, int winRow){
    std::vector<int> scores {-10, -10, -10, -10, -10, -10};
    std::vector<std::vector<signed char>> predictedPlayground;
    std::pair<int, int> bestPlay = {defaultValue, defaultValue};
    byte toDeepToDig = 0;

    for(byte counter = 0; counter < givenPlayground.size(); counter++){
        predictedPlayground = givenPlayground;
        if(columnEmpty(predictedPlayground, counter)){
            predictedPlayground[counter][findPossibleDestination(givenPlayground, counter)] = green;
            if(winControl(predictedPlayground, winRow) && winRow == 4){
                scores[counter] = win;
                break;
            }else if(lostControl(predictedPlayground, winRow)){
                scores[counter] = lose;
            } else if(drawControl(predictedPlayground)){
                scores[counter] = draw;
            } else{
                scores[counter] = Algorithm(predictedPlayground, true, depth-1, winRow);
            }
        }
    }

    toDeepToDig = 0;
    for(byte c = 0; c < scores.size(); c++)
    {
        if(scores[c] == -1)
        {
            toDeepToDig += 1;
        }
    }

    if(toDeepToDig == 6)
    {
        bestPlay.first = 6;
        bestPlay.second = 6;
        return bestPlay;
    }

    Serial.println("All moves");
    for (byte i = 0; i < scores.size(); i++){
        Serial.print(scores[i]);
        Serial.print(" ");
        if(scores[i] > bestPlay.first){
            bestPlay.first = scores[i];
            bestPlay.second = i;
        }
    }
    Serial.println("");
    return bestPlay;
}

// durchläuft einen Weg und gibt seinen wert zurück
int miniMax::Algorithm(std::vector<std::vector<signed char>> playground, bool max, int depth, int winRow){
    std::vector<std::vector<signed char>> predictedPlayground;
    bool maxPlayer = !max;
    int score = defaultValue;
    int scoreAlg;

    if(depth <= 0){
        return nothingFound;
    }

    if(maxPlayer){
        for(int columnMax = 0; columnMax < playground.size(); columnMax++){   
            predictedPlayground = playground;

            if(columnEmpty(playground, columnMax)){
                predictedPlayground[columnMax][findPossibleDestination(playground, columnMax)] = green;

                if(winControl(predictedPlayground, winRow) && winRow == 4){
                    score = win;
                    return score;
                } else if(lostControl(predictedPlayground, winRow)){
                    if (score == defaultValue){
                        score = lose;
                    }
                } else if(drawControl(predictedPlayground)){
                    if((score == defaultValue || score == lose)){
                        score = draw;
                    }
                } else{
                    scoreAlg = Algorithm(predictedPlayground, maxPlayer, depth-1, winRow);
                    if(score < scoreAlg){
                        score = scoreAlg;
                        if(score == win){
                            return score;
                        }
                    }
                }
            }
        }
    } else{
        for(int columnMin = 0; columnMin < playground.size(); columnMin++){   
            predictedPlayground = playground;

            if(columnEmpty(playground, columnMin)){
                predictedPlayground[columnMin][findPossibleDestination(playground, columnMin)] = red;

                if(winControl(predictedPlayground, winRow) && winRow == 4){
                    if(score == defaultValue){
                        score = win;
                    }
                } else if(lostControl(predictedPlayground, winRow)){
                    score = lose;
                    return score;
                } else if(drawControl(predictedPlayground)){
                    if(score == win || score == defaultValue){
                        score = draw;
                    }                    
                } else{
                    scoreAlg = Algorithm(predictedPlayground, maxPlayer, depth-1, winRow);
                    if (score > scoreAlg || score == defaultValue){
                        score = scoreAlg;
                        if(score == lose){
                            return score;
                        }
                    } 
                }
           }
        }
    }
    return score;
}

//Gibt den Wert ,,TRUE" zurück, wenn die Spalte noch spielbar ist
bool miniMax::columnEmpty(std::vector<std::vector<signed char>> playground, int column) {
    return (playground[column][0] == 0);
}

//Gibt den Wert der Zeile zürück, in der der Stein landet, wenn er fallengelassen wird
int miniMax::findPossibleDestination(std::vector<std::vector<signed char>> playground, int currentColumnNumber) {
    int tmp = -1;
    for(int i=playground[0].size()-1; i>=0;){
        if (playground[currentColumnNumber][i] != 0){
            i--;
            tmp = i-1;
        }
        else{
            tmp = i;
            return tmp;
        }
    }
    return tmp;
}

//Gibt den Wert ,,True" zurück, wenn das Spiel gewonnen ist
bool miniMax::winControl(std::vector<std::vector<signed char>> playground, int winRow){
    return (gameOver(playground, winRow) == 1);
}

//Gibt den Wert ,,True" zurück, wenn das Spiel unentschieden ist
bool miniMax::drawControl(std::vector<std::vector<signed char>> playground){
    for (byte r = 0; r < nRows; r++) {
        for (byte c = 0; c < nColumns; c++) {
            if(playground[c][r] == 0){
                return false;
            }
        }
    }
    return true;
}

//Gibt den Wert ,,True" zurück, wenn das Spiel verloren ist
bool miniMax::lostControl(std::vector<std::vector<signed char>> playground, int winRow){
    return (gameOver(playground, winRow) == -1);
}

// Gibt den Wert "True" zurück, wenn das Spiel vorbei ist, also wenn ein Spieler gewonnen hat
int miniMax::gameOver(std::vector<std::vector<signed char>> playground, int winRow){
    // Diagonale oben
    for (size_t xStart = 0, yStart = 3; xStart < 3;) {
        int lastColor = 0;
        int count = 1;
        for (size_t c = xStart, r = yStart; c < nColumns && r >= 0; c++, r--) {
            int color = playground[c][r];

            if (color == 0) {
                lastColor = color;
                count = 0;
            } else if (lastColor == 0) {
                lastColor = color;
                count = 1;
            } else if (lastColor == color) {
                count++;
                lastColor = color;
            } else {
                count = 1;
                lastColor = color;
            }

            if (count == winRow) {
                if(color == green)
                {
                    return 1;
                }
                else
                {
                    return -1;
                }
            }
        }
        if (yStart < 4) {
            yStart++;
        } else {
            xStart++;
        }
    }
    // Diagonale unten
    for (size_t xStart = 0, yStart = 1; xStart < 3;) {
        int lastColor = 0;
        int count = 1;
        for (size_t c = xStart, r = yStart; c < nColumns && r < nRows; c++, r++) {
            int color = playground[c][r];

            if (color == 0) {
                lastColor = color;
                count = 0;
            } else if (lastColor == 0) {
                lastColor = color;
                count = 1;
            } else if (lastColor == color) {
                count++;
                lastColor = color;
            } else {
                count = 1;
                lastColor = color;
            }

            if (count == winRow) {
                if(color == green)
                {
                    return 1;
                }
                else
                {
                    return -1;
                }
            }
        }
        if (yStart > 0) {
            yStart--;
        } else {
            xStart++;
        }
    }
    // Spalte
    for (size_t c = 0; c < nColumns; c++) {
        int lastColor = 0;
        int count = 0;
        for (size_t r = 0; r < nRows; r++) {
            int color = playground[c][r];

            if (color == 0) {
                lastColor = color;
                count = 0;
            } else if (lastColor == 0) {
                lastColor = color;
                count = 1;
            } else if (lastColor == color) {
                count++;
                lastColor = color;
            } else {
                count = 1;
                lastColor = color;
            }

            if (count == winRow) {
                if(color == green)
                {
                    return 1;
                }
                else
                {
                    return -1;
                }
            }
        }
    }
    // Zeile
    for (size_t r = 0; r < nRows; r++) {
        int lastColor = 0;
        int count = 0;
        for (size_t c = 0; c < nColumns; c++) {
            int color = playground[c][r];

            if (color == 0) {
                lastColor = color;
                count = 0;
            } else if (lastColor == 0) {
                lastColor = color;
                count = 1;
            } else if (lastColor == color) {
                count++;
                lastColor = color;
            } else {
                count = 1;
                lastColor = color;
            }

            if (count == winRow) {
                if(color == green)
                {
                    return 1;
                }
                else
                {
                    return -1;
                }
            }
        }
    }
    return 0;
}