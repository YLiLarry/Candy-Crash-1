#include "game.h"
#include "../public/global.h"

using namespace std;

// returns true if min <= val <= max
bool isBetween(int min, int max, int val) {
	return min <= val && val <= max;
}

// returns true if swap command has valid parameters
bool isValidSwap(int row, int col, int dir) {
	return isBetween(0, 9, row) && isBetween(0, 9, col) && isBetween(0, 3, dir);
}

// the main command interpreter
void Game::startGame() {

	board = new Board(10);
	string command;

	cin >> command;
/*
 *    while (cin >> command) {
 *
 *        if (command == "swap") {
 *
 *            int row, col, dir;
 *            cin >> row >> col >> dir;
 *
 *            // check for valid parameters
 *            if (isValidSwap(row, col, dir))
 *            board->swap(row, col, (Direction)dir);
 *        }
 *    }
 */
}
