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

		string square;
		file >> square;

		// not used at the moment
		int advanced;

		CellType type;
		Colour color;

		advanced = (square[0] == '_') ? 0 : (square[0] - '0');
		type = (square[1] == '_') ? (Colour)0 : (Colour)square[1];

		// this is another change
	}
}
