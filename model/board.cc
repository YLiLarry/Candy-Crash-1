#include "board.h"
#include "../view/textview/textview.h"

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
