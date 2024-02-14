#include <Arduino.h>
#include <vector>
#include "Color.h"
#ifndef LEDMATRIX_H
#define LEDMATRIX_H

class LEDMatrix
{
    public:

    std::vector< std::vector<Color>> LEDvalues;

    LEDMatrix(byte *, byte, byte);
    void setLightValue(int, int,  Color);
    void placeStone(int, Color);
    void setLEDs();
    void printNumber(byte);

    private:
    std::vector< std::vector<byte> > pins;
    byte currentColumn;
    byte nColumns;
    byte nRows;
    bool savedLastTime; 
    ulong lastTime;
    byte step = 1;

    bool flash(int);
};
#endif