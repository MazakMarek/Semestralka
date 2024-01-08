#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "PlayingField.h"


Body::Body(int x, int y, char texture) : x(x), y(y), texture(texture) {}

int Body::getX() {
    return this->x;
}

int Body::getY() {
    return this->y;
}

void Body::setX(int parX) {
    this->x = parX;
}

void Body::setY(int parY) {
    this->y = parY;
}


PlayingField::PlayingField() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    snake1.emplace_back(10, 10, this->snake1Text);
    snake2.emplace_back(12, 12, this->snake2Text);

    this->makeField();
}

    bool PlayingField::getGameEndHrac1() {
        return gameEndHrac1;
    }

    bool PlayingField::getGameEndHrac2(){
        return gameEndHrac2;
    }

    char PlayingField::getSmer1(){
        return smer1;
    }

    char PlayingField::getSmer2(){
        return smer2;
    }

void PlayingField::setSmer1(char posun) {
    char tempP = smer1;
    char vysP = posun;
    switch (posun) {
        case 'a':
            if (tempP == 'd') {
                vysP = smer1;
            }
            break;
        case 's':
            if (tempP == 'w') {
                vysP = smer1;
            }
            break;
        case 'd':
            if (tempP == 'a') {
                vysP = smer1;
            }
            break;
        case 'w':
            if (tempP == 's') {
                vysP = smer1;
            }
            break;
        default: vysP = smer1;
            break;
    }
    smer1 = vysP;
}

void PlayingField::setSmer2(char posun) {
    char tempP = smer2;
    char vysP = posun;
    switch (posun) {
        case 'a':
            if (tempP == 'd') {
                vysP = smer2;
            }
            break;
        case 's':
            if (tempP == 'w') {
                vysP = smer2;
            }
            break;
        case 'd':
            if (tempP == 'a') {
                vysP = smer2;
            }
            break;
        case 'w':
            if (tempP == 's') {
                vysP = smer2;
            }
            break;
        default: vysP = smer2;
            break;
    }
    smer2 = vysP;
}

    void PlayingField::makeField() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                board[i][j] = ' ';
            }
        }

        // Horne steny '-'
        for (int i = 0; i < cols; i++)
            board[0][i] = '-';

        for (auto &row : board) {
            row[0] = '|';
            row[cols - 1] = '|';
        }

        // Creating bottom walls with '-'
        for (int i = 0; i < cols; i++) {
            board[rows - 1][i] = '-';
        }

        for (Body body:snake1) { // inicializacia hada1
            board[body.getX()][body.getY()] = snake1Text;
        }

        for (Body body:snake2) { // vykreslenie hada2
            board[body.getX()][body.getY()] = snake2Text;
        }
        board[fruitX][fruitY] = '#';
    }

    void PlayingField::generateRandomFruit() {
        bool foundSpace = false;
        while (!foundSpace) {
            fruitX = 2 + std::rand() % (rows - 2);
            fruitY = 2 + std::rand() % (cols - 2);
            if (board[fruitX][fruitY] == ' ') {
                foundSpace = true;
            }
        }
    }

    void PlayingField::posunHada2() {
        int smerX = 0;
        int smerY = 0;
        switch (this->smer2) {
            case 'a': smerY = -1; break;
            case 'd': smerY = +1; break;
            case 's': smerX = +1; break;
            case 'w': smerX = -1; break;
            default: break;
        }

        int predosleX;
        int predosleY;
        bool posunPrvy = true;
        bool zjedolJedlo = false;
        for (Body& body : this->snake2) {
            if (posunPrvy) {
                predosleX = body.getX();
                predosleY = body.getY();
                body.setX(predosleX + smerX);
                body.setY(predosleY + smerY);
                posunPrvy = false;
                if (board[body.getX()][body.getY()] == '#'){
                    zjedolJedlo = true;
                }
                else if (board[body.getX()][body.getY()] != ' ') {
                    this->gameEndHrac2 = false;
                }
            } else {
                int tempX = body.getX();
                int tempY = body.getY();
                body.setX(predosleX);
                body.setY(predosleY);
                predosleX = tempX;
                predosleY = tempY;
            }
        }
        if (zjedolJedlo) {
            this->snake2.emplace_back(predosleX, predosleY, snake2Text);
            this->generateRandomFruit();
        }
    }

    void PlayingField::posunHada1() {
        int smerX = 0;
        int smerY = 0;
        switch (this->smer1) {
            case 'a': smerY = -1; break;
            case 'd': smerY = +1; break;
            case 's': smerX = +1; break;
            case 'w': smerX = -1; break;
            default: break;
        }
        int predosleX;
        int predosleY;
        bool posunPrvy = true;
        bool zjedolJedlo = false;
        for (Body& body : this->snake1) {
            if (posunPrvy) {
                predosleX = body.getX();
                predosleY = body.getY();
                body.setX(predosleX + smerX);
                body.setY(predosleY + smerY);
                posunPrvy = false;
                if (board[body.getX()][body.getY()] == '#'){
                    zjedolJedlo = true;
                }
                else if (board[body.getX()][body.getY()] != ' ') {
                    this->gameEndHrac1 = false;
                }
            } else {
                int tempX = body.getX();
                int tempY = body.getY();
                body.setX(predosleX);
                body.setY(predosleY);
                predosleX = tempX;
                predosleY = tempY;
            }
        }
        if (zjedolJedlo) {
            this->snake1.emplace_back(predosleX, predosleY, snake1Text);
            this->generateRandomFruit();
        }
    }


    std::string PlayingField::printBoard() const {
        std::string result;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result += board[i][j];
                result += " ";
            }
            result += "\n";
        }
        return result;
    }
