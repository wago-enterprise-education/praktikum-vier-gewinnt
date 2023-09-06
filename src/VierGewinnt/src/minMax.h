#ifndef MINMAX_H
#define MINMAX_H

#include <Arduino.h>
#include <vector>

class minMax
{
    public:
        void init(std::vector <std::vector<int>>);
        int Algorithm(std::vector<std::vector<int>>);

    private:
        const int nColumns = 5;
        const int nothingFound = -10;
        const int win = 1;
        const int draw = 0;
        const int lose = -1;
        const int red = 1;
        const int green = 2;
        int finish = nothingFound;
        bool turnNPC;

        std::vector <std::vector<int>> playground;

        bool columnEmpty(std::vector<std::vector<int>>, int);
        int findPossibleDestination(std::vector<std::vector<int>>, int);
        bool winControl(std::vector<std::vector<int>>);
        bool drawControl(std::vector<std::vector<int>>);
        bool lostControl(std::vector<std::vector<int>>);
};

#endif