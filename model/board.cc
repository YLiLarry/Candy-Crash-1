#include <fstream>
#include <sstream>
#include "board.h"
#include "../view/textview/textview.h"
#include "../public/global.h"

using namespace std;

Board::Board(int n, View *v) {

	grid = new Square *[n];
	for (int r = 0; r < n; r++) {
		grid[r] = new Square[n];
		for (int c = 0; c < n; c++) {
			grid[r][c].init(r, c, n, grid, v);
		}
	}

	size = n;

	level = 0;
	score = 0;
	turnScore = 0;

}

Board::~Board() {

	for (int i = 0; i < size; i++) {

		delete[] grid[i];
	}

	delete[] grid;
}

void Board::start(int l) {

	level = l;
	loadLevel(level);
}

void Board::loadLevel(int level) {

	if (level == 0) {

		#ifdef DEBUG_BOARD
				cerr << "File: ";
		
				string f;
				cin >> f;
		
				ifstream file(f.c_str());
		#else
				ifstream file("sequence.txt");
		#endif

#ifdef DEBUG_BOARD
		if (! file.good()) {throw string("unable to read the sequence file: '") + f + "'";}
#endif

		string square;

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {

				file >> square;
				Type type;

				switch (square[1]) {
					case '_': type = Basic; break;
					case 'h': type = Lateral; break;
					case 'v': type = Upright; break;
					case 'b': type = Unstable; break;
					case 'p': type = Psychedelic; break;
					default: {throw string("unexpected square type: '") + square[1] + "'";}
				}

				Colour colour = (Colour)(square[2] - '0');

				grid[i][j].setColour(colour);
				grid[i][j].setType(type);
				grid[i][j].setNeighbours();

				if (i == size - 1 && j == size - 1) {

					file >> levelZeroColours;
				}
			}
		}
	}
}

void Board::setNewSquare(Square &sq) {

	if (level == 0) {

		Colour newColour = (Colour)(levelZeroColours[0] - '0');
		Type newType = Basic;

		sq.setColour(newColour);
		sq.setType(newType);

		// recycles the colours
		char c = levelZeroColours[0];
		levelZeroColours.erase(0, 1);
		levelZeroColours += c;
	}
}

void Board::swap(int row, int col, Direction d) {

	cleared = 0;
	turnScore = 0;

	grid[row][col].swapWith(d);
	
	//view->draw();

	//clearSquares(*grid[row][col].neighbour[d]);
	//clearSquares(grid[row][col]);

	//if (!cleared) grid[row][col].swapWith(d);

	//grid[row][col].clearNotified();

	//view->draw();
}

void Board::clearAt(int row, int col) {

	clearSquares(grid[row][col]);
}

void Board::clearNotifications(int row, int col) {

	grid[row][col].clearNotified();
}

void Board::dropSquares() {

	for (int c = 0; c < size; c++) {

		grid[0][c].drop();

		while (grid[0][c].getColour() == Empty) {

			setNewSquare(grid[0][c]);
			grid[0][c].drop();
		}
	}
}

void Board::clearSquares(Square &root) {

	collectMatched(root);

	Colour backup = root.getColour();
	int radius = 0;

	if (hMatch.size() > 3 || vMatch.size() > 3) {

		radius = 4;
	} else if (hMatch.size() == 3 || vMatch.size() == 3) {

		radius = 2;
	}

	if (hMatch.size() < 3 && vMatch.size() < 3) return;

	// L match
	if (hMatch.size() == 3 && vMatch.size() == 3) {

		for (int i = 0; i < 3; i++) {
			hMatch[i]->clear(cleared, turnScore, radius);
		}
		for (int i = 0; i < 3; i++) {
			vMatch[i]->clear(cleared, turnScore, radius);
		}

		root.setColour(backup);
		root.setType(Unstable);

		return;
	}

	// horizontal
	if (hMatch.size() > vMatch.size()) {

		int n = (int)hMatch.size();

		for (int i = 0; i < n; i++) {
			hMatch[i]->clear(cleared, turnScore, radius);
		}

		switch (n) {
			case 4: root.setColour(backup);
					root.setType(Lateral); 
					break;
			case 5: root.setColour(backup);
					root.setType(Psychedelic);
					break;
		}

		return;
	} 
	
	// vertical
	if (hMatch.size() < vMatch.size()) {

		int n = (int)vMatch.size();

		for (int i = 0; i < n; i++) {
			vMatch[i]->clear(cleared, turnScore, radius);
		}

		switch (n) {
			case 4: root.setColour(backup);
					root.setType(Upright); 
					break;
			case 5: root.setColour(backup);
					root.setType(Psychedelic);
					break;
		}

		return;
	}
}

void Board::collectMatched(Square &root) {

	hMatch.clear();
	vMatch.clear();

	int row = root.getRow();
	int col = root.getCol();

	// self
	if (grid[row][col].isReady()) {
		hMatch.push_back(&root);
		vMatch.push_back(&root);
	}
	// up
	for (int r = row - 1; r >= 0; r--) {
		if (grid[r][col].isReady()) {
			vMatch.push_back(&grid[r][col]);
		}
	}
	// down
	for (int r = row + 1; r < size; r++) {
		if (grid[r][col].isReady()) {
			vMatch.push_back(&grid[r][col]);
		}
	}
	// left
	for (int c = col - 1; c >= 0; c--) {
		if (grid[row][c].isReady()) {
			hMatch.push_back(&grid[row][c]);
		}
	}
	// right
	for (int c = col + 1; c < size; c++) {
		if (grid[row][c].isReady()) {
			hMatch.push_back(&grid[row][c]);
		}
	}
}

string Board::validMove() {

	ostringstream ss;
	bool foundMatch = false;

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {
			for (int d = 0; d < 4; d++) {

				if (grid[r][c].neighbour[d]) {
					grid[r][c].swap((Direction)d);

					if (grid[r][c].isReady() ||
						grid[r][c].neighbour[d]->isReady()) {

						ss << "swap " << r << " " << c << " " << d;
						foundMatch = true;
					}

					grid[r][c].clearReady();
					grid[r][c].clearNotified();

					grid[r][c].swap((Direction)d);

					grid[r][c].clearReady();
					grid[r][c].clearNotified();

					if (foundMatch) return ss.str();
				}
			}
		}
	}

	ss << "none";
	return ss.str();
}

string Board::hint() {

	return validMove();
}

void Board::scramble() {

	for (int i = 0; i < size; i++) {
		random_shuffle(&grid[i][0], &grid[i][size - 1]);
	}

	/*for (int i = 0; i < size; i++) {*/
	//for (int j = 0; j < size; j++) {
	//view->setColour(i, j, grid[i][j].getColour());
	//view->setType(i, j, grid[i][j].getType());
	//}
	/*}*/
}


void Board::printGridInfo() {

	for (int i = 0; i < size; i++) {

		for (int j = 0; j < size; j++) {

			cerr << grid[i][j].isNotified() << grid[i][j].isReady() << " ";
		}
		cerr << endl;
	}
}
