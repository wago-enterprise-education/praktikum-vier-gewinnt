#include <Arduino.h>
#include <vector>

#ifndef LEDMATRIX_H
#define LEDMATRIX_H

class LEDMatrix
{
    public:
    std::vector< std::vector<signed char> > LEDvalues;

    LEDMatrix(byte *, byte, byte);
    void setLightValue(int, int,  byte);
    void reset();
    void setLEDs();
    void printNumber(byte);
    std::pair<int, int> findPossibleDestination(int);
    int findPossibleDestination(int, int);
    std::vector<byte> findPossibleColumns(int);
    std::vector<std::vector< std::pair<int, int > >> getBestPath();
    bool update(bool);
    bool isDraw();

    private:
    const byte off = 0;
    const byte red = 1;
    const byte green = 2;
    const byte flahRed = 3;
    const byte flashGreen = 4;

    std::vector< std::vector<byte> > pins;
    std::vector< std::pair<int, int > > winPath;
    ulong lastTime;
    byte nColumns; 
    byte nRows;
    byte currentColumn;
    byte step = 1;
    bool draw = false;
    bool won = false;
    bool savedLastTime; 

    void endAnimation();
    void drawAnimation();
    void resetAnimation();
    void rollingStone(int, int, byte);

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