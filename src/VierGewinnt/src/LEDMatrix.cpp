#include <LEDMatrix.h>
#include <Arduino.h>
#include <vector>

    LEDMatrix::LEDMatrix(int *p, int initNColumns, int initNLines){
        nColumns = initNColumns;
        nLines = initNLines;

        int tmp;
        if(initNColumns > initNLines) { tmp = initNColumns;}
        else { tmp = initNLines; }
        
        for(int i = 0, x = 0; i < 3; i++)
        {
            std::vector<int> temp3;
            for (int j = 0; j < tmp; j++)
            {
                int tmp2 = p[x];
                temp3.push_back(tmp2);
                x++;
            }
            pins.push_back(temp3);
        }

        for(int i = 0; i < initNColumns; i++)
        {
            std::vector<int> tmp2;
            for (int j = 0; j < initNLines; j++)
            {
                // if(j + i == 4){
                //     tmp2.push_back(1);
                // }
                // else{
                    tmp2.push_back(off);
                // }
                
            }
            LEDvalues.push_back(tmp2);
        }

        currentColumn = 0;
    }

    void LEDMatrix::setLightValue(int currentColumnnumber, int previousColumnnumber,  int color){
        
        LEDvalues[previousColumnnumber][0] = off;
        if (color < 3){
            for(int i = 0; i < nLines; i++)
            {
            if(LEDvalues.at(currentColumnnumber).at(i) == 0)
                {
                    LEDvalues[currentColumnnumber][i] = color;
                    break;
                }
            }
        }
        else {
            LEDvalues[currentColumnnumber][0] = color;
        }
    }

    bool LEDMatrix::possibleDestination(int currentColumnnumber){
        return (LEDvalues.at(currentColumnnumber).at(0) == off);
    }

    int LEDMatrix::findPossibleDestination(int currentColumnnumber, int direction){
        int tmp = -1;
        for (size_t i = currentColumnnumber; i < nColumns && i >= 0; i = i + direction)
        {
            if(possibleDestination(i)){
                return i;
            }
        }
        
        return tmp;
    }

    void LEDMatrix::reset(){
        for(int i = 0; i < nColumns; i++)
        {
            for (int j = 0; j < nLines; j++)
            {
                LEDvalues[i][j] = 0;
            }
        }
    }

    bool LEDMatrix::update(){
        bool won = winControl();
        if (dropDown){

        }
        if(won){
            endAnimation();
        }
        else{
            setLEDs();
        }
        return won;
    }

    void LEDMatrix::setLEDs(){
        for (byte l = 0; l < nLines; l++)
        {
            digitalWrite(pins[red][l], LOW);
            digitalWrite(pins[green][l], LOW);
        }
    
        for (byte c = 0; c < nColumns; c++)
        {
            digitalWrite(pins[0][c], c != currentColumn);
        }
      
        for (int line = 0; line < nLines; line++)
        {
            switch (LEDvalues[currentColumn][line])
            {
            case 0: //off
                digitalWrite(pins[red][line], LOW);
                digitalWrite(pins[green][line], LOW);
                break;
      
            case 1: //red
                digitalWrite(pins[red][line], HIGH);
                break;

            case 2: //green
                digitalWrite(pins[green][line], HIGH);
                break;
                
            case 3: //flash red
                digitalWrite(pins[red][0], HIGH);
                break;

            case 4: //flash green
                digitalWrite(pins[green][0], HIGH);
                break;
            }
        }
        currentColumn = (currentColumn + 1) % 6;
    }

    bool LEDMatrix::winControl(){
        // Zeilen Prüfen
        for (size_t c = 0; c < nColumns; c++)
        {
            int lastColor = off;
            int count = 1;
            for (size_t l = 0; l < nLines; l++)
            {
                int color = LEDvalues[c][l];
                if(lastColor == off){
                    lastColor = color;
                    count = 1;
                }
                else if(lastColor == color){
                    count++;
                    lastColor = color;
                }
                else{
                    count = 1;
                    lastColor = color;
                }

                if(count == 4){
                    return HIGH;
                }
            }
        }

        // Spalten prüfen
        for (size_t l = 0; l < nLines; l++)
        {
            int lastColor = off;
            int count = 1;
            for (size_t c = 0; c < nColumns; c++)
            {
                int color = LEDvalues[c][l];
                if(lastColor == off){
                    lastColor = color;
                    count = 1;
                }
                else if(lastColor == color){
                    count++;
                    lastColor = color;
                }
                else{
                    count = 1;
                    lastColor = color;
                }

                if(count == 4){
                    return HIGH;
                }
            }
        }

        // diagonale oben prüfen
        for (size_t xStart = 0, yStart = 3; xStart < 3; )
        {
            int lastColor = off;
            int count = 1;
            for (size_t c = xStart, l = yStart; c < nColumns && l >= 0; c++, l--)
            {
                int color = LEDvalues[c][l];
                if(lastColor == off){
                    lastColor = color;
                    count = 1;
                }
                else if(lastColor == color){
                    count++;
                    lastColor = color;
                }
                else{
                    count = 1;
                    lastColor = color;
                }

                if(count == 4){
                    return HIGH;
                }
            }

            if(yStart < 4){
                yStart++;
            }
            else{
                xStart++;
            }
        }
        

        // diagonale unten prüfen
        for (size_t xStart = 0, yStart = 1; xStart < 3; )
        {
            int lastColor = off;
            int count = 1;
            for (size_t c = xStart, l = yStart; c < nColumns && l >= 0; c++, l++)
            {
                int color = LEDvalues[c][l];
                if(lastColor == off){
                    lastColor = color;
                    count = 1;
                }
                else if(lastColor == color){
                    count++;
                    lastColor = color;
                }
                else{
                    count = 1;
                    lastColor = color;
                }

                if(count == 4){
                    return HIGH;
                }
            }

            if(yStart > 0){
                yStart--;
            }
            else{
                xStart++;
            }
        }
        
        return LOW;
    }

    void LEDMatrix::endAnimation(){
        
    }
