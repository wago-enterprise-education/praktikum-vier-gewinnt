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
                    tmp2.push_back(0);
                // }
                
            }
            LEDvalues.push_back(tmp2);
        }

        currentColumn = 0;
    }

    void LEDMatrix::setLightValue(int currentColumnumber, int color){
        if (LEDvalues.at(currentColumnumber).at(0) == off){
            if (color < 2){
                for(int i = nLines-1; i >= 0; i--)
                {
                    if(LEDvalues.at(currentColumnumber).at(i) == 0)
                    {
                        LEDvalues[currentColumnumber][i] = color;
                    }
                }
            }
            else {
                LEDvalues[currentColumnumber][0] = color;
            }
        }
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
        // bool won = winControl();
        // if(won){
        //     endAnimation();
        //     delay(2000);
        // }
        // else{
        //     setLEDs();
        // }
        setLEDs();
        return LOW;//won;
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
        int count = 0;
        int currentColor = off;
        // Spalten prüfen
        for (size_t i = 0; i < nColumns; i++)
        {
            for (size_t j = nLines-1; j >= 0 && count + j >= 4; j--)
            {
                int color = LEDvalues[i][j];
                if(color == currentColor && currentColor != off){
                    count++;
                }
                else{
                    count = 1;
                    currentColor = color;
                }
                if(count == 4){
                    endAnimation();
                    return HIGH;
                }
            }
        }

        count = 0;
        currentColor = off;
        // Zeilen prüfen
        for (size_t i = nLines; i >= 0; i--)
        {
            for (size_t j = 0; j < nColumns && count + j < 4; j++)
            {
                int color = LEDvalues[i][j];
                if(color == currentColor && currentColor != off){
                    count++;
                }
                else{
                    count = 1;
                    currentColor = color;
                }
                if(count == 4){
                    endAnimation();
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
                int color = LEDvalues[i][j];
                if(color == currentColor && currentColor != off){
                    count++;
                }
                else{
                    count = 1;
                    currentColor = color;
                }
                if(count == 4){
                    endAnimation();
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
        i = 1;
        j = 0;
        while(i >= 0 && j <= 2){
            for(int x = j, y = i; x <= 5 && y <= 4; x++, y++){
                int color = LEDvalues[i][j];
                if(color == currentColor && currentColor != off){
                    count++;
                }
                else{
                    count = 1;
                    currentColor = color;
                }
                if(count == 4){
                    endAnimation();
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
    // bool winControl(std::pair<int, int> currentPos, std::pair<int, int> lastPos, int lastColor, int count, int direction){
    //     if(count + currentPos.second >= 4 && count + currentPos.first < 4){
    //         if(direction == 0 || direction == 2 ){}
    //         int currentColor = LEDvalues[currentPos.first][currentPos.second];
    //         if(currentColor == lastColor){
    //             count++;
    //         }
    //     }
    // }

    void LEDMatrix::endAnimation(){
    
    }
