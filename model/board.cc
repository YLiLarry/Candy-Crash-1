#include <fstream>
#include "board.h"
#include "../view/textview/textview.h"
#include "../public/global.h"

using namespace std;

Board::Board(int n) : size(n) {
	// initializing grid
	grid = new Square *[size];

	for (int i = 0; i < size; i++) {
		grid[i] = new Square[size];
	}

	level = 0;
	score = 0;
	//movesRemain = ??
}

void Board::loadLevel(int level) {

	if (level == 0) {

		ifstream file("sequence.txt");

		char strSquare;
		file >> strSquare;

		int advanced;

		// THIS IS A TEST
	}
}
