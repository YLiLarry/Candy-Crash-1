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

	turnScore = 0;

	grid[row][col].swapWith(d);
	view->draw();

	printGridInfo();

	bool m1 = clearSquares(*grid[row][col].neighbour[d]);
	bool m2 = clearSquares(grid[row][col]);

	if (m1 || m2) {
		cerr << "turn score: " << turnScore << endl;
	} else {
		grid[row][col].swap(d);
	}

	grid[row][col].unNotifyNeighbours();

	printGridInfo();

	view->draw();
}

void _emptyColours(vector<Square *> matched, Square *except) {

	int n = matched.size();

	for (int i = 0; i < n; i++) {

		if (matched[i] != except) {
			matched[i]->setColour(Empty);
		}
		matched[i]->ready = false;
	}
}

bool Board::clearSquares(Square &root) {
	hMatch.clear();
	vMatch.clear();

	int row = root.row;
	int col = root.col;

	// left
	for (int c = col - 1; c >= 0; c--) {
		if (grid[row][c].ready) {
			hMatch.push_back(&grid[row][c]);
		}
	}
	// self
	if (grid[row][col].ready) {
		hMatch.push_back(&grid[row][col]);
	}
	// right
	for (int c = col + 1; c < size; c++) {
		if (grid[row][c].ready) {
			hMatch.push_back(&grid[row][c]);
		}
	}
	// up
	for (int r = row - 1; r >= 0; r--) {
		if (grid[r][col].ready) {
			vMatch.push_back(&grid[r][col]);
		}
	}
	// self
	if (grid[row][col].ready) {
		vMatch.push_back(&grid[row][col]);
	}
	// down
	for (int r = row + 1; r < size; r++) {
		if (grid[r][col].ready) {
			vMatch.push_back(&grid[r][col]);
		}
	}

	if (hMatch.size() == 3 && vMatch.size() == 3) {
		cerr << "L MATCH" << endl;

		_emptyColours(hMatch, &root); 
		_emptyColours(vMatch, &root);

		root.setType(Unstable);
		
		turnScore += 15;

		return true;
	}

	if (hMatch.size() >= 3 && vMatch.size() < 3) {
		cerr << "HORIZONTAL MATCH" << endl;


		if (vMatch.size() == 4) {
			cerr << "LATERAL SQUARE" << endl; 
			_emptyColours(hMatch, &root);
			root.setType(Lateral);
			turnScore += 8;
		} else if (vMatch.size() == 5) {
			cerr << "PSYCHEDELIC SQUARE" << endl;
			_emptyColours(hMatch, &root);
			root.setType(Psychedelic);
			turnScore += 15;
		} else {
			_emptyColours(hMatch, NULL);
			turnScore += 3;
		}

		return true;
	}

	if (hMatch.size() < 3 && vMatch.size() >=3) {
		cerr << "VERTICAL MATCH" << endl;

		if (vMatch.size() == 4) {
			cerr << "UPRIGHT SQUARE" << endl;
			_emptyColours(vMatch, &root);
			root.setType(Upright);
		} else if (vMatch.size() == 5) {
			cerr << "PSYCHEDELIC SQUARE" << endl;
			_emptyColours(vMatch, &root);
			root.setType(Psychedelic);
		} else {
			_emptyColours(vMatch, NULL);
			turnScore += 3;
		}

		return true;
	}

	if (hMatch.size() < 3 && vMatch.size() < 3) {
		cerr << "NO MATCH" << endl;

		return false;
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
