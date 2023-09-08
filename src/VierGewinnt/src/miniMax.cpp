#include <Arduino.h>
#include <miniMax.h>
#include <vector>

std::pair<int, int> miniMax::run(std::vector<std::vector<int>> givenPlayground)
{
    std::vector<int> scores {-10, -10, -10, -10, -10, -10};
    std::vector<std::vector<int>> predictedPlayground;

    for(int counter = 0; counter < givenPlayground.size(); counter++)
    {
        predictedPlayground = givenPlayground;

        if(columnEmpty(predictedPlayground, counter))
        {
            predictedPlayground[counter][findPossibleDestination(givenPlayground, counter)] = green;
            
            if(winControl(predictedPlayground))
            {
                scores[counter] = win;
            }
            else if(lostControl(predictedPlayground))
            {
                scores[counter] = lose;
            }
            else if(drawControl(predictedPlayground))
            {
                scores[counter] = draw;
            }
            else
            {
                scores[counter] = Algorithm(predictedPlayground, true);
            }
        }
    }

    std::pair<int, int> bestPlay = {-10, -10};

    for (int i = 0; i < scores.size(); i++)
    {
        if(scores[i] > bestPlay.first){
            bestPlay.first = scores[i];
            bestPlay.second = i;
        }
    }
    

    return bestPlay;
}


int miniMax::Algorithm(std::vector<std::vector<int>> playground, bool max)
{
    std::vector<std::vector<int>> predictedPlayground;
    bool maxPlayer = !max;
    int score;
    int scoreAlg;
    score = nothingFound;

    if(maxPlayer)
    {
        for(int columnMax = 0; columnMax < playground.size(); columnMax++)
        {   
            predictedPlayground = playground;

            if(columnEmpty(playground, columnMax))
            {
                predictedPlayground[columnMax][findPossibleDestination(playground, columnMax)] = green;

                if(winControl(predictedPlayground))
                {
                    score = win;
                    return score;
                }
                else if(lostControl(predictedPlayground))
                {
                    if (score == nothingFound)
                    {
                        score = lose;
                    }
                }
                else if(drawControl(predictedPlayground))
                {
                    if((score == nothingFound || score == lose))
                    {
                        score = draw;
                    }
                }
                else
                {
                    scoreAlg = Algorithm(predictedPlayground, maxPlayer);
                    if(scoreAlg > score)
                    {
                        score = scoreAlg;
                        if(score == win){
                            return score;
                        }
                    }
                    
                }
            }
        }
    }
    else
    {
        for(int columnMin = 0; columnMin < playground.size(); columnMin++)
        {   
            predictedPlayground = playground;

            if(columnEmpty(playground, columnMin))
            {
                predictedPlayground[columnMin][findPossibleDestination(playground, columnMin)] = red;

                if(winControl(predictedPlayground))
                {
                    if(score == nothingFound)
                    {
                        score = win;
                    }
                }
                else if(lostControl(predictedPlayground))
                {
                    score = lose;
                    return score;
                }
                else if(drawControl(predictedPlayground))
                {
                    if(score == win || score == nothingFound)
                    {
                        score = draw;
                    }                    
                }
                else
                {
                    scoreAlg = Algorithm(predictedPlayground, maxPlayer);
                    if (scoreAlg < score || score == nothingFound)
                    {
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
bool miniMax::columnEmpty(std::vector<std::vector<int>> playground, int column) {
    if(playground[column][0] == 0){
        return true;
    }
    
    return false;
}


//Gibt den Wert der Zeile zürück, in der der Stein landet, wenn er fallengelassen wird
int miniMax::findPossibleDestination(std::vector<std::vector<int>> playground, int currentColumnNumber) {
    
    int tmp = -1;
    for(int i=playground[0].size()-1; i>=0;){
        if (playground[currentColumnNumber][i] != 0){
            i--;
            tmp = i-1;
        }
        else{
            tmp = i;
            return tmp;
            break;
        }
    }
    return tmp;
}


//Gibt den Wert ,,True" zurück, wenn das Spiel gewonnen ist
bool miniMax::winControl(std::vector<std::vector<int>> playground){
    if(gameOver(playground) == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//Gibt den Wert ,,True" zurück, wenn das Spiel unentschieden ist
bool miniMax::drawControl(std::vector<std::vector<int>> playground){
    for (size_t r = 0; r < nRows; r++) {
        for (size_t c = 0; c < nColumns; c++) {
            if(playground[c][r] == 0){
                return false;
            }
        }
    }
    return true;
}


//Gibt den Wert ,,True" zurück, wenn das Spiel verloren ist
bool miniMax::lostControl(std::vector<std::vector<int>> playground){
    if(gameOver(playground) == -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}



int miniMax::gameOver(std::vector<std::vector<int>> playground)
{
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

            if (count == 4) {
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

            if (count == 4) {
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

            if (count == 4) {
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

            if (count == 4) {
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