#include <Arduino.h>
#include <minMax.h>
#include <vector>

void minMax::init(std::vector<std::vector<int>> givenPlayground)
{
    minMax::playground = givenPlayground;
    finish = nothingFound;
    turnNPC = true;
}


int minMax::Algorithm(std::vector<std::vector<int>> playground)
{
    std::vector<std::vector<int>> predictedPlayground = playground;

    for(int column = 0; column <= nColumns; column++)
    {   
        predictedPlayground = playground;

        if(columnEmpty(playground, column))
        {
            if(turnNPC)
            {
                predictedPlayground[column][findPossibleDestination(playground, column)] = green;
            }
            else
            {
                predictedPlayground[column][findPossibleDestination(playground, column)] = red;
            }


            if(winControl(predictedPlayground) < win)
            {
                finish = win;
            }
            else if(drawControl(predictedPlayground) && finish < draw)
            {
                finish = draw;
            }
            else if(lostControl(predictedPlayground) && finish < lose)
            {
                finish = lose;
            }
        }
    }

    if(finish != nothingFound)
    {
        return finish;
    }

    turnNPC = !turnNPC;
    return minMax::Algorithm(predictedPlayground);
}


//Gibt den Wert ,,TRUE" zurück, wenn die Spalte noch spielbar ist
bool minMax::columnEmpty(std::vector<std::vector<int>> playground, int column) {
}


//Gibt den Wert der Zeile zürück, in der der Stein landet, wenn er fallengelassen wird
int minMax::findPossibleDestination(std::vector<std::vector<int>> playground, int currentColumnNumber) {
}


//Gibt den Wert ,,True" zurück, wenn das Spiel gewonnen ist
bool minMax::winControl(std::vector<std::vector<int>> playground){
}


//Gibt den Wert ,,True" zurück, wenn das Spiel unentschieden ist
bool minMax::drawControl(std::vector<std::vector<int>> playground){
}


//Gibt den Wert ,,True" zurück, wenn das Spiel verloren ist
bool minMax::lostControl(std::vector<std::vector<int>> playground){
}
