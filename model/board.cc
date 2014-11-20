#include <fstream>
#include "board.h"
#include "../view/textview/textview.h"

using namespace std;

Board::Board() {
	// initializing grid
	grid = new Square *[10];

	for (int i = 0; i < 10; i++) {
		grid[i] = new Square[10];

	}

	level = 0;
	score = 0;
	//movesRemain = ??
}

void Board::loadLevel(int level) {

	if (level == 0) {

		ifstream file("sequence.txt");

		for (int i = 0; i < 
	}
}
