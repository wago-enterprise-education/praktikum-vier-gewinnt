#ifndef MINIMAX_H
#define MINIMAX_H

#include <Arduino.h>
#include <vector>

class miniMax{
    public:
        std::pair<int, int> run(std::vector <std::vector<signed char>>, int, int);
        int Algorithm(std::vector<std::vector<signed char>>, bool, int, int);

    private:
        const int defaultValue = -10;
        const int win = 1;
        const int draw = 0;
        const int lose = -5;
        const int nothingFound = -1;
        const byte red = 1;
        const byte green = 2;

        byte nColumns = 6;     
        byte nRows = 5;        

        bool columnEmpty(std::vector<std::vector<signed char>>, int);
        bool winControl(std::vector<std::vector<signed char>>, int);
        bool drawControl(std::vector<std::vector<signed char>>);
        bool lostControl(std::vector<std::vector<signed char>>, int);
        int gameOver(std::vector<std::vector<signed char>>, int);
        int findPossibleDestination(std::vector<std::vector<signed char>>, int);
};

#endif