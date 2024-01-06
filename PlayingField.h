// PlayingField.h
#ifndef PLAYINGFIELD_H
#define PLAYINGFIELD_H

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <mutex>

class Body {
private:
    int x;
    int y;
    char texture;

public:
    Body(int x, int y, char texture);

    int getX() const;
    int getY() const;
    void setX(int parX);
    void setY(int parY);
};

class PlayingField {
private:
    std::mutex mutex;

    bool gameEnd = true;

    std::vector<Body> snake1;
    std::vector<Body> snake2;

    char smer1 = 'd';
    char smer2 = 'a';

    char snake1Text = 'o';
    char snake2Text = 'x';

    static const int rows = 20;
    static const int cols = 31;

    char board[rows][cols];
    int fruitX = 10, fruitY = 15;

public:
    PlayingField();

    void makeField();
    void generateRandomFruit();
    void lockGameState();
    void unlockGameState();
    bool isGameEnd() const;

    void posunHada1();
    void posunHada2();

    std::string printBoard() const;

    char getSmer1();
    char getSmer2();
    void setSmer1(char smer);
    void setSmer2(char smer);
};

#endif // PLAYINGFIELD_H
