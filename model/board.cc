#include <fstream>
#include <sstream>
#include "board.h"
#include "../view/textview/textview.h"
#include "../public/global.h"

using namespace std;

/*
 * Board constructor
 * n is for building an n x n grid
 */
Board::Board(int n) : size(n) {
	// initializing grid
	grid = new Square *[size];
	for (int i = 0; i < size; i++) {
		grid[i] = new Square[size];
	}

	level = 0;
	score = 0;
	turnScore = 0;

	view = new View(size);

	loadLevel(level);
	view->draw();
}

/*
 * Board destructor
 */
Board::~Board() {
	for (int i = 0; i < size; i++) {
		delete[] grid[i];
	}
	delete[] grid;

	delete view;
}

/*
 * loadLevel(level) changes current level
 * to match the level parameter
 *
 * level 0 is loaded from sequence.txt
 */
void Board::loadLevel(int level) {
	if (level == 0) {

#ifdef MATCH_TEST
		cerr << "file name: ";
		string f;
		cin >> f;
		ifstream file(("levels/" + f).c_str());
#else
		ifstream file("sequence.txt");
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
				}

				Colour colour = (Colour)(square[2] - '0');

				grid[i][j].row = i;
				grid[i][j].col = j;
				grid[i][j].colour = colour;
				grid[i][j].type = type;
				grid[i][j].view = view;

				// setting neighbours

				if (i - 1 >= 0) 
					grid[i][j].neighbour[Up] = &grid[i - 1][j];
				if (i + 1 < size) 
					grid[i][j].neighbour[Down] = &grid[i + 1][j];
				if (j - 1 >= 0) 
					grid[i][j].neighbour[Left] = &grid[i][j - 1];
				if (j + 1 < size) 
					grid[i][j].neighbour[Right] = &grid[i][j + 1];

				view->setColour(i, j, colour);
				view->setType(i, j, type);
				view->setScore(score);
				view->setLevel(level);
			}
		}
	}
}


void Board::printGridInfo() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cerr << grid[i][j].notified << grid[i][j].ready << " ";
		}
		cerr << endl;
	}
}

void Board::swap(int row, int col, Direction d) {

	//turnScore = 0;
	//nMatch = 0;

	grid[row][col].swapWith(d);
	view->draw();

	printGridInfo();

	bool swap = clearSquares(*grid[row][col].neighbour[d]);
	bool swapee = clearSquares(grid[row][col]);

	if (swap || swapee) {
		//cerr << "turn score: " << turnScore << endl;
		cerr << "nMatch = " <<  nMatch << endl;
	} else {
		grid[row][col].swap(d);
	}

	grid[row][col].unNotifyNeighbours();

	printGridInfo();

	view->draw();
}

void Board::clearRow(int row) {

	for (int c = 0; c < size; c++) {
		cerr << "--clearing row--" << endl;
		clearSquare(grid[row][c]);
	}
}

void Board::clearCol(int col) {


	for (int r = 0; r < size; r++) {
		cerr << "--clearing column--" << endl;
		clearSquare(grid[r][col]);
	}
}

void Board::clearRad(int row, int col, int rad) {


	int rMin = (row - rad >= 0)? row - rad : 0;
	int rMax = (row + rad < size)? row + rad : size - 1;
	int cMin = (col - rad >= 0)? col - rad : 0;
	int cMax = (col + rad < size)? col + rad : size - 1;

	for (int r = rMin; r <= rMax; r++) {
		for (int c = cMin; c <= cMax; c++) {
			cerr << "--clearing radius of size " << rad << "--" << endl;
			clearSquare(grid[r][c]);
		}
	}
}

void Board::clearColour(Colour c) {


	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {

			cerr << "clearing all colour: " << c << endl;
			cerr << "checking (" << grid[i][j].getRow() << "," << grid[i][j].getCol() << ")" << endl;

			if (grid[i][j].getColour() == c) {
				cerr << "matched (" << grid[i][j].getRow() << "," << grid[i][j].getCol() << ")" << endl;
				clearSquare(grid[i][j]);
			}
		}
	}
}

void Board::emptyColours(vector<Square *> matched) {

	for (int i = 0; i < (int)matched.size(); i++) {
		if (matched[i]->ready) {
			clearSquare(*matched[i]);
		}
	}
}

void Board::appendMatchVectors(Square &root) {
	hMatch.clear();
	vMatch.clear();

	int row = root.row;
	int col = root.col;

	// self
	if (grid[row][col].ready) {
		hMatch.push_back(&root);
		vMatch.push_back(&root);
	}
	// up
	for (int r = row - 1; r >= 0; r--) {
		if (grid[r][col].ready) {
			vMatch.push_back(&grid[r][col]);
		}
	}
	// down
	for (int r = row + 1; r < size; r++) {
		if (grid[r][col].ready) {
			vMatch.push_back(&grid[r][col]);
		}
	}
	// left
	for (int c = col - 1; c >= 0; c--) {
		if (grid[row][c].ready) {
			hMatch.push_back(&grid[row][c]);
		}
	}
	// right
	for (int c = col + 1; c < size; c++) {
		if (grid[row][c].ready) {
			hMatch.push_back(&grid[row][c]);
		}
	}
}

void Board::clearSquare(Square &sq) {

	cerr << "============================================" << endl << endl;



	
	Colour backupColour = sq.getColour();
	sq.setColour(Empty); // also sets on view
	sq.ready = false;

	nMatch++;

	if (sq.getType() == Lateral) {

		cerr << "LATERAL SQUARE" << endl;

		sq.setType(Basic);
		clearRow(sq.getRow());

	} else if (sq.getType() == Upright) {

		cerr << "UPRIGHT SQUARE" << endl;

		sq.setType(Basic);
		clearCol(sq.getCol());

	} else if (sq.getType() == Unstable) {

		cerr << "UNSTABLE SQUARE" << endl;

		int rad;

		if (hMatch.size() > vMatch.size()) {
			rad = hMatch.size();
		} else {
			rad = vMatch.size();
		}

		sq.setType(Basic);
		clearRad(sq.getRow(), sq.getCol(), rad);

	} else if (sq.getType() == Psychedelic) {

		cerr << "PSYCHELIC SQUARE" << endl;

		sq.setType(Basic);
		clearColour(backupColour);
	}

	cerr << "clearing square (" << sq.getRow() << "," << sq.getCol() << ")" << endl;
	view->draw();
	cerr << "type " << sq.getType() << endl;
	cerr << "============================================" << endl << endl;

}

bool Board::clearSquares(Square &root) {

	appendMatchVectors(root);

	// no match
	if (hMatch.size() < 3 && vMatch.size() < 3) return false;

	if (hMatch.size() == 3 && vMatch.size() == 3) {
		cerr << "L MATCH" << endl;

		Colour backup = hMatch[0]->getColour();

		emptyColours(hMatch);
		emptyColours(vMatch);

		root.setColour(backup);
		root.setType(Unstable);

		return true;
	} else if (hMatch.size() >= 3 && vMatch.size() < 3) {
		cerr << "HORIZONTAL MATCH" << endl;


		if (hMatch.size() == 4) {

			cerr << "LATERAL SQUARE" << endl; 

			Colour backup = hMatch[0]->getColour();

			for (int i = 0; i < (int)hMatch.size(); i++) { 
				clearSquare(*hMatch[i]);
			}

			//emptyColours(hMatch);

			root.setColour(backup);
			root.setType(Lateral);

		} else if (hMatch.size() == 5) {

			cerr << "PSYCHEDELIC SQUARE" << endl;

			Colour backup = hMatch[0]->getColour();

			emptyColours(hMatch);

			root.setColour(backup);
			root.setType(Psychedelic);

		} else {

			for (int i = 0; i < (int)hMatch.size(); i++) {
				clearSquare(*hMatch[i]);
			}
		}

		return true;
	} else if (hMatch.size() < 3 && vMatch.size() >=3) {
		cerr << "VERTICAL MATCH" << endl;

		if (vMatch.size() == 4) {

			cerr << "UPRIGHT SQUARE" << endl;

			Colour backup = vMatch[0]->getColour();

			emptyColours(vMatch);

			root.setCol(backup);
			root.setType(Upright);

		} else if (vMatch.size() == 5) {

			cerr << "PSYCHEDELIC SQUARE" << endl;

			Colour backup = vMatch[0]->getColour();

			emptyColours(vMatch);

			root.setColour(backup);
			root.setType(Psychedelic);
		} else {
			
			emptyColours(vMatch);

		}

		return true;
	}

	// redundant
	cerr << "YOU SHOULD NEVER SEE THIS" << endl;
	return false;
}

/*
 *string Board:: validMove() {
 *    #if DEBUG_BOARD
 *        fprintf(stderr,"BOARD:: validMove()\n");
 *    #endif
 *    ostringstream ss;
 *    Board b(this->size);
 *    for (int i = 0; i < this->size; i++) {
 *        for (int j = 0; j < this->size; j++) {
 *            b.grid[i][j] = this->grid[i][j];
 *        }
 *    }
 *    for (int i = 0; i < this->size; i++) {
 *        for (int j = 0; j < this->size; j++) {
 *            b.swapMechanism(i,j,Up);
 *            if (! b.findMatches(i,j).empty()) {
 *            	ss << i << " " << j << " " << (int)Up;
 *            	return ss.str();
 *            };
 *            b.swapMechanism(i,j,Up);
 *            b.swapMechanism(i,j,Down);
 *            if (! b.findMatches(i,j).empty()) {
 *            	ss << i << " " << j << " " << (int)Down;
 *            	return ss.str();
 *            };
 *            b.swapMechanism(i,j,Down);
 *            b.swapMechanism(i,j,Left);
 *            if (! b.findMatches(i,j).empty()) {
 *            	ss << i << " " << j << " " << (int)Left;
 *            	return ss.str();
 *            };
 *            b.swapMechanism(i,j,Left);
 *            b.swapMechanism(i,j,Right);
 *            if (! b.findMatches(i,j).empty()) {
 *            	ss << i << " " << j << " " << (int)Right;
 *            	return ss.str();
 *            };
 *        }
 *    }
 *    return string();
 *}
 *
 *bool Board:: hasMove() {
 *    return this->validMove().length();
 *}
 *
 *void Board:: hint() {
 *    string str = validMove();
 *    #if DEBUG_BOARD
 *        fprintf(stderr,"BOARD hint(%s)\n",str.c_str());
 *    #endif
 *    view->print(str);
 *}
 */

void Board::scramble() {

	for (int i = 0; i < size; i++) {
		random_shuffle(&grid[i][0], &grid[i][size - 1]);
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			view->setColour(i, j, grid[i][j].colour);
			view->setType(i, j, grid[i][j].type);
		}
	}

	view->draw();
}
