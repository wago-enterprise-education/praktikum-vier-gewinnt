#include <vector>
#include <Arduino.h>
#ifndef LEDMATRIX_H
#define LEDMATRIX_H

class LEDMatrix
{
    public:
    LEDMatrix(int *, int, int);
    void setLightValue(int, int,  int);
    void setLightValue(int, int);
    void reset();
    bool update();
    int findPossibleDestination(int, int);
    bool dropDown;
    void rollingStone(int, int, int);

    

    private:
    std::vector< std::vector<int> > pins;
    std::vector< std::vector<int> > LEDvalues;
    int nColumns; 
    int nLines;
    int currentColumn;
    std::vector< std::pair<int, int > > winPath;

    const int off = 0;
    const int red = 1;
    const int green = 2;
    const int flahRed = 3;
    const int flashGreen = 4; 

    void setLEDs();
    bool winControl();
    bool winControlRow();
    bool winControlColumn();
    bool winControlDiagonalUpwards();
    bool winControlDiagonalDownwards();
    void endAnimation();
    bool possibleDestination(int);  
    bool flash(int);
    void drawViso();
    bool drawControl();
    void resetAnimation();

    ulong lastTime;
    bool savedLastTime;
    int step = 1;
    bool draw;
    bool won = false;
};
#endif