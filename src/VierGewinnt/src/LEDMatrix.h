#ifndef LEDMATRIX_H
#define LEDMATRIX_H
class LEDMatrix{
    public:
        LEDMatrix(int*, int, int);
        void setLightValue(int, int);
        void reset();
        bool update();
    private:    
        void setLEDs();
        bool winControl();
        void endAnimation();
};
#endif