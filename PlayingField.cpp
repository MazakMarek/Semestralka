// PlayingField.cpp
#include "PlayingField.h"

// Body Class Implementation

Body::Body(int x, int y, char texture) : x(x), y(y), texture(texture) {}

int Body::getX() const {
    return x;
}

int Body::getY() const {
    return y;
}

void Body::setX(int parX) {
    x = parX;
}

void Body::setY(int parY) {
    y = parY;
}

// PlayingField Class Implementation

PlayingField::PlayingField() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    snake1.emplace_back(10, 10, snake1Text);
    snake2.emplace_back(12, 12, snake2Text);

    makeField();
}

void PlayingField::makeField() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = ' ';
        }
    }

    for (int i = 0; i < cols; i++)
        board[0][i] = '-';

    for (auto &row : board) {
        row[0] = '|';
        row[cols - 1] = '|';
    }

    for (const Body &body : snake1) {
        board[body.getX()][body.getY()] = snake1Text;
    }

    for (const Body &body : snake2) {
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
    board[fruitX][fruitY] = '#';
}

void PlayingField::lockGameState() {
    mutex.lock();
}

void PlayingField::unlockGameState() {
    mutex.unlock();
}

bool PlayingField::isGameEnd() const {
    return gameEnd;
}

void PlayingField::posunHada1() {
    int smerX = 0;
    int smerY = 0;
    switch (smer1) {
        case 'a':
            smerY = -1;
            break;
        case 'd':
            smerY = +1;
            break;
        case 's':
            smerX = +1;
            break;
        case 'w':
            smerX = -1;
            break;
        default:
            break;
    }

    int predosleX;
    int predosleY;
    bool posunPrvy = true;
    for (Body &body : snake1) {
        if (posunPrvy) {
            predosleX = body.getX();
            predosleY = body.getY();
            body.setX(predosleX + smerX);
            body.setY(predosleY + smerY);
            posunPrvy = false;
            if (board[body.getX()][body.getY()] != ' ' && board[body.getX()][body.getY()] != '#') {
                gameEnd = false;
            }
        } else {
            predosleX = body.getX();
            predosleY = body.getY();
            body.setX(predosleX);
            body.setY(predosleY);
        }
    }

    if (board[snake1.front().getX()][snake1.front().getY()] == '#') {
        generateRandomFruit();
    }
}

void PlayingField::posunHada2() {
    int smerX = 0;
    int smerY = 0;
    switch (smer2) {
        case 'a':
            smerY = -1;
            break;
        case 'd':
            smerY = +1;
            break;
        case 's':
            smerX = +1;
            break;
        case 'w':
            smerX = -1;
            break;
        default:
            break;
    }

    int predosleX;
    int predosleY;
    bool posunPrvy = true;
    for (Body &body : snake2) {
        if (posunPrvy) {
            predosleX = body.getX();
            predosleY = body.getY();
            body.setX(predosleX + smerX);
            body.setY(predosleY + smerY);
            posunPrvy = false;
            if (board[body.getX()][body.getY()] != ' ') {
                gameEnd = false;
            }
        } else {
            predosleX = body.getX();
            predosleY = body.getY();
            body.setX(predosleX);
            body.setY(predosleY);
        }
    }

    if (board[snake2.front().getX()][snake2.front().getY()] == '#') {
        generateRandomFruit();
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

char PlayingField::getSmer1() {
    return this->smer1;
}

char PlayingField::getSmer2() {
    return this->smer2;
}

void PlayingField::setSmer1(char smer) {
    this->smer1 = smer;
}

void PlayingField::setSmer2(char smer) {
    this->smer2 = smer;
}
