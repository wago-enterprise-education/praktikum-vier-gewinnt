#include <vector>
#include <Arduino.h>
#ifndef LEDMATRIX_H
#define LEDMATRIX_H

class LEDMatrix
{
    public:
    LEDMatrix(int *, int, int);
    void setLightValue(int, int,  int);
    void reset();
    bool update();
    int findPossibleDestination(int, int);

    private:
    std::vector< std::vector<int> > pins;
    std::vector< std::vector<int> > LEDvalues;
    std::vector< std::pair<int, int > > winPath;
    ulong lastTime;
    int nColumns; 
    int nLines;
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
    bool winControlRow();
    bool winControlColumn();
    bool winControlDiagonalUpwards();
    bool winControlDiagonalDownwards();
    bool winControl();
    bool possibleDestination(int);  
    bool flash(int);
    bool drawControl();
};
#endif