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

void Board::setNeighbours(Square *s) {

	int row = s->row;
	int col = s->col;

	if (row - 1 >= 0) s->neighbours[Up] = &grid[row][col - 1];
	if (row + 1 < size) s->neighbours[Down] = &grid[row][col + 1];
	if (col - 1 >= 0) s->neighbours[Left] = &grid[row - 1][col];
	if (col + 1 < size) s->neighbours[Right] = &grid[row + 1][col];
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


				grid[i][j].neighbours[Up] = (i - 1 >= 0)? &grid[i - 1][j] : 0;
				grid[i][j].neighbours[Down] =  (i + 1 < size)? &grid[i + 1][j] : 0;
				grid[i][j].neighbours[Left] = (j - 1 >= 0)? &grid[i][j - 1] : 0;
				grid[i][j].neighbours[Right] = (j + 1 < size)? &grid[i][j + 1] : 0;

				//setNeighbours(&grid[i][j]);

				view->setColour(i, j, colour);
				view->setType(i, j, type);
				view->setScore(score);
				view->setLevel(level);
			}
		}
	}
}


void Board::swap(int row, int col, Direction d) {

	grid[row][col].swapWith(d);
	view->draw();

	clearSquares(*grid[row][col].neighbour[d]);
	clearSquares(grid[row][col]);

	view->draw();
}

/*
 *bool isBasicHorizontal(Square &root) {
 *    int sum = 0;
 *}
 *
 *bool isUnstable(Square &root) {
 *    if (root.neighbour[Up] && root.neighbour[Right]) {
 *
 *    } else if (root.neighbour[Up] && root.neighbour[Left]) {
 *
 *    } else if (root.neighbour[Down] && root.neighbour[Right]) {
 *
 *    } else if (root.neighbour[Down] && root.neighbour[Left]) {
 *
 *    } else {
 *        return false;
 *    }
 *}
 */

void _clearVec(vector<Square *> match) {

	for (int i = 0; i < (int)match.size(); i++) {
		cerr << "clearing: (" << match[i]->row << "," << match[i]->col << ")" << endl;

		if (match[i]->type == Basic) {
			match[i]->setColour(Empty);
			match[i]->notified = false;
			match[i]->ready = false;
		} else if (match[i]->type == Lateral) {

		} else if (match[i]->type == Upright) {

		} else if (match[i]->type == Unstable) {

		} else if (match[i]->type == Psychedelic) {

		}
	}
}

void Board::clearSquares(Square &root) {
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

		_clearVec(hMatch);
		_clearVec(vMatch);

		root.setType(Unstable);
	}

	if (hMatch.size() >= 3 && vMatch.size() < 3) {
		cerr << "HORIZONTAL MATCH" << endl;

		_clearVec(hMatch);

		if (vMatch.size() == 4) {
			cerr << "LATERAL SQUARE" << endl; 
			root.setType(Lateral);
		} else if (vMatch.size() == 5) {
			cerr << "PSYCHEDELIC SQUARE" << endl;
			root.setType(Psychedelic);
		}
	}

	if (hMatch.size() < 3 && vMatch.size() >=3) {
		cerr << "VERTICAL MATCH" << endl;

		_clearVec(vMatch);

		if (vMatch.size() == 4) {
			cerr << "UPRIGHT SQUARE" << endl;
			root.setType(Upright);
		} else if (vMatch.size() == 5) {
			cerr << "PSYCHEDELIC SQUARE" << endl;
			root.setType(Psychedelic);
		}
	}

	if (hMatch.size() < 3 && vMatch.size() < 3) {
		cerr << "NO MATCH" << endl;
	}
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
