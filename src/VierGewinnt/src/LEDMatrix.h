#include <vector>
#include <Arduino.h>
#ifndef LEDMATRIX_H
#define LEDMATRIX_H

class LEDMatrix
{
    public:
    LEDMatrix(int *, int, int);
    void setLightValue(int, int,  int);
    void setLightValue(int, int, int, int);
    void reset();
    bool update(bool);
    int findPossibleDestination(int, int);
    void printNumber(int);
    std::pair<int, int> findPossibleDestination(int);
    std::vector<int> findPossibleColumns(int);
    std::vector<std::vector< std::pair<int, int > >> getBestPath();

    private:
    std::vector< std::vector<int> > pins;
    std::vector< std::vector<int> > LEDvalues;
    std::vector< std::pair<int, int > > winPath;
    ulong lastTime;
    int nColumns; 
    int nRows;
    int currentColumn;
    int step = 1;
    bool draw;
    bool won = false;
    bool savedLastTime;

    const int off = 0;
    const int red = 1;
    const int green = 2;
    const int flahRed = 3;
    const int flashGreen = 4; 

    void setLEDs();
    void endAnimation();
    void drawViso();
    void resetAnimation();
    void rollingStone(int, int, int);
    std::vector<std::pair<int, int>> winControlRow();
    std::vector<std::pair<int, int>> winControlColumn();
    std::vector<std::pair<int, int>> winControlDiagonalUpwards();
    std::vector<std::pair<int, int>> winControlDiagonalDownwards();
    bool winControl();
    bool possibleDestination(int);  
    bool flash(int);
    bool drawControl();
};
#endif