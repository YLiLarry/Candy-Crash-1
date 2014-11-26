#include "game.h"
#include "../public/global.h"
#include <sstream>

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
	view = new View(size);
	board = new Board(size, view);

	board->start(0);

	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {

			view->setColour(row, col, board->grid[row][col].colour);
			view->setType(row, col, board->grid[row][col].type);
		}
	}

	view->draw();

	string cmd;

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

	Direction direction = (Direction)dir;
	
	// check for valid parameters
	if (isValidSwap(row, col, direction)) {

		board->swap(row, col, direction);

		view->draw();

		switch (direction) {
			case Up:    board->clearAt(row - 1, col); break;
			case Down:  board->clearAt(row + 1, col); break;
			case Left:  board->clearAt(row, col - 1); break;
			case Right: board->clearAt(row, col + 1); break;
		}

		board->clearAt(row, col);

		board->clearNotifications(row, col);

		view->draw();
		if (board->cleared == 0) {
			
			view->print("no match");
			board->swap(row, col, direction);
		} else {
			
			ostringstream ss;
			ss << "cleared:  " << board->cleared << endl;
			ss << "score  : +" << board->turnScore << endl;

			view->print(ss.str());
		}

		board->dropSquares();

		view->draw();
	}
}

void Game::levelUp() {board->loadLevel(++board->level);}
void Game::levelDown() {if (board->level > 0) {board->loadLevel(--board->level);}}
void Game::restart() {board->loadLevel(board->level);}
