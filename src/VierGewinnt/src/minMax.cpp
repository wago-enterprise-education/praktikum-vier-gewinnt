#include <Arduino.h>
#include <minMax.h>
#include <vector>

std::vector<int> minMax::run(std::vector<std::vector<int>> givenPlayground)
{
    std::vector<int> scores;
    scores.reserve(givenPlayground.size());

    for(int counter = 0; counter < givenPlayground.size(); counter++)
    {
        scores[counter] = Algorithm(givenPlayground, false);
    }

    return scores;
}


int minMax::Algorithm(std::vector<std::vector<int>> playground, bool max)
{
    std::vector<std::vector<int>> predictedPlayground;
    predictedPlayground.reserve(playground.size());
    predictedPlayground[0].reserve(playground[0].size());   //Funktioniert das so?
    bool maxPlayer = !max;
    int score = nothingFound;

    if(maxPlayer)
    {
        for(int columnMax = 0; columnMax <= playground.size(); columnMax++)
        {   
            //predictedPlayground[columnMax].reserve(playground[0].size());     //Alternative!
            predictedPlayground = playground;

            if(columnEmpty(playground, columnMax))
            {
                predictedPlayground[columnMax][findPossibleDestination(playground, columnMax)];

                if(winControl(predictedPlayground) && score < win)
                {
                    score = win;
                }
                else if(drawControl(predictedPlayground) && score < draw)
                {
                    score = draw;
                }
                else if(lostControl(predictedPlayground) && score < lose)
                {
                    score = lose;
                }
                else
                {
                    score = Algorithm(predictedPlayground, maxPlayer);
                }
            }
        }
    }
    else
    {
        for(int columnMin = 0; columnMin <= playground.size(); columnMin++)
        {   
            //predictedPlayground[columnMax].reserve(playground[0].size());     //Alternative
            predictedPlayground = playground;

            if(columnEmpty(playground, columnMin))
            {
                predictedPlayground[columnMin][findPossibleDestination(playground, columnMin)];

                if(winControl(predictedPlayground) && score == nothingFound)
                {
                    score = win;
                }
                else if(drawControl(predictedPlayground) && score > draw)
                {
                    score = draw;
                }
                else if(lostControl(predictedPlayground) && score < lose)
                {
                    score = lose;
                }
                else
                {
                    score = Algorithm(predictedPlayground, maxPlayer);
                }
            }
        }
    }
    return score;
}


//Gibt den Wert ,,TRUE" zurück, wenn die Spalte noch spielbar ist
bool minMax::columnEmpty(std::vector<std::vector<int>> playground, int column) {
    if(playground[column][0] == 0){
        return true;
    }
    
    return false;
}


//Gibt den Wert der Zeile zürück, in der der Stein landet, wenn er fallengelassen wird
int minMax::findPossibleDestination(std::vector<std::vector<int>> playground, int currentColumnNumber) {
}


//Gibt den Wert ,,True" zurück, wenn das Spiel gewonnen ist
bool minMax::winControl(std::vector<std::vector<int>> playground){
    bool win = false;
    int winCounter = 0;

    for(int c = 0; c <= nColumns; c++)
    {

    }
}


//Gibt den Wert ,,True" zurück, wenn das Spiel unentschieden ist
bool minMax::drawControl(std::vector<std::vector<int>> playground){
}


//Gibt den Wert ,,True" zurück, wenn das Spiel verloren ist
bool minMax::lostControl(std::vector<std::vector<int>> playground){
}
