#include <LEDMatrix.h>
#include <Arduino.h>
#include <vector>

class LEDMatrix
{
    const int off = 0;
    const int red = 1;
    const int green = 2;
    const int flashRed = 3;
    const int flashGreen = 4;

    std::vector< std::vector<int> > pins;
    std::vector< std::vector<byte> > LEDs;
    byte cColumns; 
    byte cLines;
    byte columnN;

    public: 
    LEDMatrix(int *p[], int pinsColumn, int pinsLine){
        cColumns = pinsColumn;
        cLines = pinsLine;

        int a;
        if(pinsColumn > pinsLine) { a = pinsColumn;}
        else { a = pinsLine; }
        for(int i = 0; i < 2; i++)
        {
            for (int j = 0; j < a; j++)
            {
                pins[i][j] = p[i][j];
            }
        }

        for(int i = 0; i < pinsColumn; i++)
        {
            for (int j = 0; j < pinsLine; j++)
            {
                LEDs[i][j] = 0;
            }
        }

        columnN = 0;
    }

    void flashLight(int columnNumber, int color)
    {

    }

    void setNewLight(int columnNumber, int color){
        for(int i = cLines-1; i >= 0; i--)
        {
            if(LEDs[columnNumber][i] == 0)
            {
                LEDs[columnNumber][i] = color;
            }
        }
    }

    void reset(){
        for(int i = 0; i < cColumns; i++)
        {
            for (int j = 0; j < cLines; j++)
            {
                LEDs[i][j] = 0;
            }
        }
    }

    bool update(){
        bool won = wincontrol();
        if(won){
            gameOver();
        }
        else{
            setLEDMatrix();
        }
        return won;
    }

    private:
    void setLEDMatrix(){
        for (byte l = 0; l < 6; l++)
        {
            digitalWrite(pins[1][l], LOW);
        }
    
        for (byte c = 0; c < 6; c++)
        {
            digitalWrite(pins[0][c], c != columnN);
        }
      
        for (int line = 0; line < 5; line++)
        {
            switch (LEDs[columnN][line])
            {
            case 0:
                digitalWrite(pins[1][line], LOW);
                break;
      
            case 1:
                digitalWrite(pins[1][line], red);
                break;
            case 2:
                digitalWrite(pins[1][line], green);
                break;
            }
        }
        columnN = (columnN + 1) % 6;
    }

    bool wincontrol(){
        int count = 0;
        int currentColor = off;
        // Spalten prüfen
        for (size_t i = 0; i < cColumns; i++)
        {
            for (size_t j = cLines-1; j >= 0 && count + j >= 4; j--)
            {
                int color = LEDs[i][j];
                if(color == currentColor && currentColor != off){
                    count++;
                }
                else{
                    count = 1;
                    currentColor = color;
                }
                if(count == 4){
                    gameOver();
                    return HIGH;
                }
            }
        }

        count = 0;
        currentColor = off;
        // Zeilen prüfen
        for (size_t i = cLines; i >= 0; i--)
        {
            for (size_t j = 0; j < cColumns && count + j < 4; j++)
            {
                int color = LEDs[i][j];
                if(color == currentColor && currentColor != off){
                    count++;
                }
                else{
                    count = 1;
                    currentColor = color;
                }
                if(count == 4){
                    gameOver();
                    return HIGH;
                }
            }
        }

        count = 0;
        currentColor = off;
        // diagonale prüfen oben 
        int i = 3;
        int j = 0;
        while(i <= 4 && j <= 2){
            for(int x = j, y = i; x <= 5 && y >= 0; x++, y--){
                int color = LEDs[i][j];
                if(color == currentColor && currentColor != off){
                    count++;
                }
                else{
                    count = 1;
                    currentColor = color;
                }
                if(count == 4){
                    gameOver();
                    return HIGH;
                }
            }
            if(i < 4){
                i++;
            }
            else{
                j++;
            }
        }

        count = 0;
        currentColor = off;
        // diagonale prüfen unten 
        int i = 1;
        int j = 0;
        while(i >= 0 && j <= 2){
            for(int x = j, y = i; x <= 5 && y <= 4; x++, y++){
                int color = LEDs[i][j];
                if(color == currentColor && currentColor != off){
                    count++;
                }
                else{
                    count = 1;
                    currentColor = color;
                }
                if(count == 4){
                    gameOver();
                    return HIGH;
                }
            }
            if(i > 0){
                i--;
            }
            else{
                j++;
            }
        }
        
        return LOW;
    }

    // direction 0 = diagonal unten, 1 = rechts, 2 = diagonal oben, 3 = oben  
    // bool wincontrol(std::pair<int, int> currentPos, std::pair<int, int> lastPos, int lastColor, int count, int direction){
    //     if(count + currentPos.second >= 4 && count + currentPos.first < 4){
    //         if(direction == 0 || direction == 2 ){}
    //         int currentColor = LEDs[currentPos.first][currentPos.second];
    //         if(currentColor == lastColor){
    //             count++;
    //         }
    //     }
    // }

    void gameOver(){
        
    }
};