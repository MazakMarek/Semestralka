#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

class Body{
private:
    int x;
    int y;
    char texture;
public:
    Body(int x, int y, char texture) : x(x), y(y), texture(texture) {}

    int getX() {
        return this->x;
    }
    int getY() {
        return this->y;
    }

    void setX(int parX) {
        this->x = parX;
    }

    void setY(int parY) {
        this->y = parY;
    }
};

class PlayingField {
private:
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
    PlayingField(){
        std::srand(static_cast<unsigned int>(std::time(0)));

        snake1.emplace_back(10,10, this->snake1Text);
        snake2.emplace_back(12,12, this->snake2Text);
        //generateRandomFruit();
        this->makeField();
    }

    void makeField() {
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

        for (Body body:snake1) { // vykreslenie hada1
            board[body.getX()][body.getY()] = snake1Text;
        }

        for (Body body:snake2) { // vykreslenie hada2
            board[body.getX()][body.getY()] = snake2Text;
        }
        board[fruitX][fruitY] = '#';
    }

    void generateRandomFruit() {
        bool foundSpace = false;
        while (!foundSpace) {
            fruitX = 2 + std::rand() % (rows - 2);
            fruitY = 2 + std::rand() % (cols - 2);
            if (board[fruitX][fruitY] == ' ') {
                foundSpace = true;
            }
        }
    }

    std::string printBoard() const {
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
};