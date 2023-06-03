#include <iostream>
#include "Board.h"
using namespace std;

int main()
{
    Board* board = nullptr;
    string command;
    while (cin >> command) {
        if (command == "LOAD_GAME_BOARD") {
            board = new Board();
            cout << board->get_board_state() << endl << endl;
        }
        else if (command == "PRINT_GAME_BOARD") {
            board->print_board();
        }
        else if (command == "DO_MOVE") {
            string arg;
            cin >> arg;
            string from = arg.substr(0, arg.find('-'));
            string to = arg.substr(arg.find('-') + 1);
            cout << board->do_move(from, to) << endl << endl;
        }
    }
}
