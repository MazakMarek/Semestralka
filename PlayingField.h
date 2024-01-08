#ifndef PLAYINGFIELD_H
#define PLAYINGFIELD_H

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

// Trieda reprezentujúca jednu časť hada
class Body {
private:
    int x;            // X-ová pozícia časti hada na hracej ploche
    int y;            // Y-ová pozícia časti hada na hracej ploche
    char texture;     // Textúra reprezentujúca časť hada

public:
    Body(int x, int y, char texture);   // Konštruktor pre inicializáciu časti hada

    int getX();     // Metóda na získanie X-ovej pozície
    int getY();     // Metóda na získanie Y-ovej pozície

    void setX(int parX);   // Metóda na nastavenie novej X-ovej pozície
    void setY(int parY);   // Metóda na nastavenie novej Y-ovej pozície
};

// Trieda reprezentujúca hraciu plochu
class PlayingField {
private:
    bool gameEndHrac1 = true;   // Premenná indikujúca koniec hry pre hráča 1
    bool gameEndHrac2 = true;   // Premenná indikujúca koniec hry pre hráča 2

    std::vector<Body> snake1;   // Vektor obsahujúci časti hada pre hráča 1
    std::vector<Body> snake2;   // Vektor obsahujúci časti hada pre hráča 2

    char smer1 = 'd';   // Smer pohybu pre hráča 1
    char smer2 = 'a';   // Smer pohybu pre hráča 2

    char snake1Text = 'o';   // Textúra reprezentujúca časti hada pre hráča 1
    char snake2Text = 'x';   // Textúra reprezentujúca časti hada pre hráča 2

    static const int rows = 20;   // Počet riadkov hracej plochy
    static const int cols = 31;   // Počet stĺpcov hracej plochy

    char board[rows][cols];   // Hracia plocha reprezentovaná dvoma rozmerným poľom
    int fruitX = 10, fruitY = 15;   // Pozícia ovocia na hracej ploche

public:
    PlayingField();   // Konštruktor pre inicializáciu hracej plochy

    bool getGameEndHrac1();   // Metóda na získanie informácie o konci hry pre hráča 1
    bool getGameEndHrac2();   // Metóda na získanie informácie o konci hry pre hráča 2

    char getSmer1();   // Metóda na získanie smeru pohybu pre hráča 1
    char getSmer2();   // Metóda na získanie smeru pohybu pre hráča 2

    void setSmer1(char posun);   // Metóda na nastavenie nového smeru pohybu pre hráča 1
    void setSmer2(char posun);   // Metóda na nastavenie nového smeru pohybu pre hráča 2

    void makeField();   // Metóda na inicializáciu hracej plochy
    void generateRandomFruit();   // Metóda na generovanie náhodného ovocia

    void posunHada2();   // Metóda na pohyb hada pre hráča 2
    void posunHada1();   // Metóda na pohyb hada pre hráča 1

    std::string printBoard() const;   // Metóda na získanie reprezentácie hracej plochy vo forme reťazca
};

#endif // PLAYINGFIELD_H