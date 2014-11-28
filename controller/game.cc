#include "game.h"
#include "../public/global.h"

using namespace std;

// returns true if min <= val <= max
bool isBetween(int min, int max, int val) {
	return min <= val && val <= max;
}

// returns true if swap cmd has valid parameters
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

Game::Game(int size) {

	// if (board) {delete board;}
	board = new Board(size);
	board->start();

	string cmd;

	cout << "command: ";
	while (cin >> cmd) {

        if (cmd == "//") {
            cin.ignore(1024, '\n');
            continue;
        } else
		if (cmd == "swap") {
			this->swap();
		} else if (cmd == "hint") {
			this->hint();
		} else if (cmd == "scramble") {
			this->scramble();
		} else if (cmd == "levelup") {
			this->levelUp();
		} else if (cmd == "leveldown") {
			this->levelDown();
		} else if (cmd == "restart") {
			this->restart();
		} else if (cmd == "quit") {
			break;
		}

		switch (board->level) {

			case 1: board->loadLevel(1); break;
		}

		cout << "command: ";
	}
}

Game::~Game() {
	delete this->board;
}

void Game::hint() {board->hint();}
void Game::scramble() {board->scramble();}
void Game::swap() {
	int row, col, dir;
	cin >> row >> col >> dir;
	
	// check for valid parameters
	if (isValidSwap(row, col, (Direction)dir)) {
		board->swap(row, col, (Direction)dir);
	}
}

void Game::levelUp() {board->loadLevel(++board->level);}
void Game::levelDown() {if (board->level > 0) {board->loadLevel(--board->level);}}
void Game::restart() {board->loadLevel(board->level);}
