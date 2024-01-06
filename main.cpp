#include <iostream>
#include "PlayingField.cpp"
#include "SnakeGame.cpp"


//int main() {
//    PlayingField playingField;
//    std::cout << playingField.printBoard();
//    return 0;
//}

#include <iostream>
#include <conio.h> // alebo <curses.h> pre Linux

int main() {
    char key;

    while (true) {
        key = _getch(); // pre Windows, použi getch() pre Linux

        switch (key) {
            case 'w': // šípka hore
                std::cout << "Hore" << std::endl;
                break;
            case 's': // šípka dole
                std::cout << "Dole" << std::endl;
                break;
            case 'a': // šípka vľavo
                std::cout << "Vlavo" << std::endl;
                break;
            case 'd': // šípka vpravo
                std::cout << "Vpravo" << std::endl;
                break;
            case 27: // Escape klávesa - ukončí program
                return 0;
            default:
                break;
        }
    }

    return 0;
}

