#ifndef LEDMATRIX_H
#define LEDMATRIX_H
class LEDMatrix{
    public:
        LEDMatrix(int*, int, int);
        void setLEDMatrix();
        void setNewLigth(int, int);
        void reset();
        bool update();
};
#endif