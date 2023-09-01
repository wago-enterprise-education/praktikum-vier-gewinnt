#include <vector>
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
    bool dropDown;

    

    private:
    std::vector< std::vector<int> > pins;
    std::vector< std::vector<int> > LEDvalues;
    int nColumns; 
    int nLines;
    int currentColumn;

    const int off = 0;
    const int red = 1;
    const int green = 2;
    const int flahRed = 3;
    const int flashGreen = 4; 

    void setLEDs();
    bool winControl();
    void endAnimation();
    bool possibleDestination(int);  
    bool flash(int);

    ulong lastTime;
    bool savedLastTime;
    int step = 1;
};
#endif