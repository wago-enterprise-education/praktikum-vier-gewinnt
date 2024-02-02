#include <Arduino.h>
#include <vector>
#include <Color.h>
#ifndef LEDMATRIX_H
#define LEDMATRIX_H

class LEDMatrix
{
    public:

    std::vector< std::vector<Color>> LEDvalues;

    LEDMatrix(byte *, byte, byte);
    void setLightValue(int, int,  Color);
    void reset(byte);
    void setLEDs();
    void printNumber(byte);
    std::pair<int, int> findPossibleDestination(int);
    int findPossibleDestination(int, int);
    std::vector<byte> findPossibleColumns(int);
    std::vector<std::vector< std::pair<int, int > >> getBestPath();
    bool update(bool);
    bool isDraw();

    private:
    std::vector< std::vector<byte> > pins;
    std::vector< std::pair<int, int > > winPath;
    Color winColor;
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
    void rollingStone(int, int, Color);

    std::vector<std::pair<int, int>> winControlRow();
    std::vector<std::pair<int, int>> winControlColumn();
    std::vector<std::pair<int, int>> winControlDiagonalUpwards();
    std::vector<std::pair<int, int>> winControlDiagonalDownwards();
    bool winControl();
    bool winControl(byte x, byte y, byte direction, byte count, Color color);
    bool possibleDestination(int);  
    bool flash(int);
    bool drawControl();
};
#endif