#include <fstream>
#include <sstream>
#include "board.h"
#include "../view/textview/textview.h"
#include "../public/global.h"

using namespace std;

Board::Board(int n) {

	grid = new Square *[n];
	for (int r = 0; r < n; r++) {
		grid[r] = new Square[n];
		for (int c = 0; c < n; c++) {
			grid[r][c].init(r, c, n, grid, view);
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

	delete view;
}

void Board::start() {

	view = new View(size);

	loadLevel(level);
	view->draw();
}

void Board::loadLevel(int level) {
	if (level == 0) {

		ifstream file("sequence.txt");
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
				}

				Colour colour = (Colour)(square[2] - '0');

				grid[i][j].setColour(colour);
				grid[i][j].setType(type);
				grid[i][j].setNeighbours();

				view->setScore(score);
				view->setLevel(level);
			}
		}
	}
}

void Board::swap(int row, int col, Direction d) {

	cleared = 0;
	turnScore = 0;

	grid[row][col].swapWith(d);
	view->draw();

	clearSquares(*grid[row][col].neighbour[d]);
	clearSquares(grid[row][col]);


	if (cleared) {
		cerr << "cleared: " << cleared << endl;
		cerr << "turn score: " << turnScore << endl;
	} else {
		grid[row][col].swap(d);
	}

	grid[row][col].clearNotifications();

	view->draw();
}

int Board::clearSquares(Square &root) {

	collectMatched(root);

	Colour backup = root.getColour();

	if (hMatch.size() < 3 && vMatch.size() < 3) {
		
		view->print("no match");
		return false;

	} else if (hMatch.size() == 3 && vMatch.size() == 3) {

		view->print("L match");

		for (int i = 0; i < 3; i++) {
			clear(*hMatch[i]);
		}
		for (int i = 0; i < 3; i++) {
			clear(*vMatch[i]);
		}

		root.setColour(backup);
		root.setType(Unstable);

	} else if (hMatch.size() >= 3 && vMatch.size() < 3) {
	
		view->print("Horizontal match");

		if (hMatch.size() == 4) {

			view->print("Lateral");

			for (int i = 0; i < 4; i++) { 
				clear(*hMatch[i]);
			}

			root.setColour(backup);
			root.setType(Lateral);

		} else if (hMatch.size() == 5) {

			view->print("Psychedelic");

			for (int i = 0; i < 5; i++) {
			
			}

			root.setColour(backup);
			root.setType(Psychedelic);

		} else {

			for (int i = 0; i < 3; i++) {
				clear(*hMatch[i]);
			}
		}
	} else if (hMatch.size() < 3 && vMatch.size() >=3) {

		view->print("Vertical match");

		if (vMatch.size() == 4) {

			view->print("Upright");

			for (int i = 0; i < 4; i++) {
				clear(*vMatch[i]);
			}

			root.setCol(backup);
			root.setType(Upright);

		} else if (vMatch.size() == 5) {

			view->print("Psychedelic");

			Colour backup = vMatch[0]->getColour();

			for (int i = 0; i < 5; i++) {
				clear(*vMatch[i]);
			}

			root.setColour(backup);
			root.setType(Psychedelic);

		} else {

			for (int i = 0; i < 3; i++) {
				clear(*vMatch[i]);
			}
		}

	}

	return true;
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

void Board::clear(Square &sq) {

	Colour tColour = sq.getColour();
	Type tType = sq.getType();

	if (tColour == Empty) return;

	sq.setColour(Empty);
	sq.setType(Basic);
	sq.setReady(false);

	cleared++;

	if (cleared == 3) turnScore = 3;
	if (cleared == 4) turnScore = 8;
	if (cleared == 5) turnScore = 15;
	if (cleared > 5) turnScore = 4 * cleared;

	if (tType == Lateral) {

		view->print("LATERAL");
		clearRow(sq.getRow());

	} else if (tType == Upright) {

		view->print("UPRIGHT");
		clearCol(sq.getCol());

	} else if (tType == Unstable) {

		int rad = (hMatch.size() > 3 || vMatch.size() > 3)? 5 : 3;
		clearRad(sq.getRow(), sq.getCol(), rad);

	} else if (tType == Psychedelic) {

		view->print("PSYCHEDELIC");
		clearColour(tColour);
	}

	view->draw();
}

void Board::clearRow(int row) {

	for (int c = 0; c < size; c++) {
		view->print("clearing row");
		clear(grid[row][c]);
	}
}

void Board::clearCol(int col) {

	for (int r = 0; r < size; r++) {
		view->print("clearing column");
		clear(grid[r][col]);
	}
}

void Board::clearRad(int row, int col, int rad) {


	int sz = size; // looks pretty

	int rMin = (row - rad >= 0)? row - rad : 0;
	int rMax = (row + rad < sz)? row + rad : sz - 1;
	int cMin = (col - rad >= 0)? col - rad : 0;
	int cMax = (col + rad < sz)? col + rad : sz - 1;

	for (int r = rMin; r <= rMax; r++) {
		for (int c = cMin; c <= cMax; c++) {
			clear(grid[r][c]);
		}
	}
}

void Board::clearColour(Colour c) {

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (grid[i][j].getColour() == c) {
				clear(grid[i][j]);
			}
		}
	}
}


string Board:: validMove() {
	#if DEBUG_BOARD
		fprintf(stderr,"BOARD:: validMove()\n");
	#endif
	ostringstream ss;
	Board b(this->size);

	for (int i = 0; i < this->size; i++) {
		for (int j = 0; j < this->size; j++) {
			b.grid[i][j] = this->grid[i][j];
		}
	}

	for (int i = 0; i < this->size; i++) {
		for (int j = 0; j < this->size; j++) {
			for (int d = 0; d < 4; d++) {

				b.grid[i][j].swap((Direction)d);
				b.collectMatched(*b.grid[i][j].neighbour[(Direction)d]);
				b.collectMatched(b.grid[i][j]);

				if (b.hMatch.size() >= 3 || b.vMatch.size() >= 3) {
					ss << i << " " << j << " " << d;
					return ss.str();
				} else {

				}
			}
		}
	}
	return string();
}

bool Board:: hasMove() {
	return this->validMove().length();
}

void Board:: hint() {
	string str = validMove();
	#if DEBUG_BOARD
		fprintf(stderr,"BOARD hint(%s)\n",str.c_str());
	#endif
	view->print(str);
}

void Board::scramble() {

	for (int i = 0; i < size; i++) {
		random_shuffle(&grid[i][0], &grid[i][size - 1]);
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			view->setColour(i, j, grid[i][j].getColour());
			view->setType(i, j, grid[i][j].getType());
		}
	}

	view->draw();
}


void Board::printGridInfo() {

	for (int i = 0; i < size; i++) {

		for (int j = 0; j < size; j++) {

			cerr << grid[i][j].isNotified() << grid[i][j].isReady() << " ";
		}
		cerr << endl;
	}
}
