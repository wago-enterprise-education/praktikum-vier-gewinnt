#ifndef MINIMAX_H
#define MINIMAX_H

#include <Arduino.h>
#include <vector>

class miniMax
{
    public:
        std::pair<int, int> run(std::vector <std::vector<signed char>>, int);
        int Algorithm(std::vector<std::vector<signed char>>, bool, int);

    private:
        int nColumns = 6;     //Muss noch variabel gehalten werden
        int nRows = 5;        //...
        const int nothingFound = -10;
        const int win = 1;
        const int draw = 0;
        const int lose = -1;
        const int red = 1;
        const int green = 2;

        bool columnEmpty(std::vector<std::vector<signed char>>, int);
        int findPossibleDestination(std::vector<std::vector<signed char>>, int);
        bool winControl(std::vector<std::vector<signed char>>);
        bool drawControl(std::vector<std::vector<signed char>>);
        bool lostControl(std::vector<std::vector<signed char>>);
        int gameOver(std::vector<std::vector<signed char>>);
};

#endif