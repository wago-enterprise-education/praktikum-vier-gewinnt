#ifndef MINIMAX_H
#define MINIMAX_H

#include <Arduino.h>
#include <vector>

class miniMax{
    public:
        /**
         * Berechnen des besten Spielzugs mithilfe des MiniMax-Algorithmus
         * @param givenPlayground Das zu berechnende Spielfeld
         * @param depth Bestimmt wie tief der Algorithmus den Spielverlauf berechnen soll
         * @param winRow Gibt an, ab wie vielen Steinen in einer Reihe das Spiel verloren ist
         * @param return Pair.First entspricht den optimalen Wert des Spielverlaufs. Pair.Second entspricht die dazugehörige Spalte
        */
        std::pair<int, int> run(std::vector<std::vector<signed char>> givenPlayground, int depth, int winRow);

    private:
        const int defaultValue = -10;
        const int win = 1;
        const int draw = 0;
        const int lose = -5;
        const int nothingFound = -1;
        const byte red = 1;
        const byte green = 2;

        byte nColumns = 6;     
        byte nRows = 5;        

        /**
         * Rekursive Funktion, Berechnung des MiniMax-Algorithmus
         * @param playground Das zu berechnende Spielfeld
         * @param max Sollte immer dann "True" sein, wenn der Gegenspieler als nächste Berechnung durchgeführt werden soll
         * @param depth Bestimmt wie tief der Algorithmus den Spielverlauf berechnen soll
         * @param winRow Bestimmt, ab wieviele Steine in einer Reihe das Spiel verloren, gewonnen ist
        */
        int Algorithm(std::vector<std::vector<signed char>> playground, bool max, int depth, int winRow);
        bool columnEmpty(std::vector<std::vector<signed char>>, int);
        bool winControl(std::vector<std::vector<signed char>>, int);
        bool drawControl(std::vector<std::vector<signed char>>);
        bool lostControl(std::vector<std::vector<signed char>>, int);
        int gameOver(std::vector<std::vector<signed char>>, int);
        int findPossibleDestination(std::vector<std::vector<signed char>>, int);
};

#endif