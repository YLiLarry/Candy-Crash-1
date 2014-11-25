#include "game.h"
#include "../public/global.h"

using namespace std;

// returns true if min <= val <= max
bool isBetween(int min, int max, int val) {
	return min <= val && val <= max;
}

// returns true if swap command has valid parameters
bool isValidSwap(int row, int col, Direction dir) {

	bool validRow = isBetween(0, 9, row);
	bool validCol = isBetween(0, 9, col);

	switch (dir) {
		case Up: row--; break;
		case Down: row++; break;
		case Left: col--; break;
		case Right: col++; break;
	}

	bool validDir = isBetween(0, 9, row) && isBetween(0, 9, col);

	return validRow && validCol && validDir;
}

// the main command interpreter
void Game::startGame() {

	board = new Board(10);
	string command;

	while (cin >> command) {

		if (command == "swap") {

			int row, col, dir;
			cin >> row >> col >> dir;

			// check for valid parameters
			if (isValidSwap(row, col, (Direction)dir)) {
				board->swap(row, col, (Direction)dir);
			}

		} else if (command == "hint") {
			
			board->hint();

		} else if (command == "scramble") {

			board->scramble();

		}
	}
}
