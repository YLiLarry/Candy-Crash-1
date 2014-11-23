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

	view = new View(size);
	loadLevel(level);
	view->draw();
}

Board::~Board() {
	for (int i = 0; i < size; i++) {
		delete[] grid[i];
	}
	delete[] grid;

	delete view;
}

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

void Board::swap(int row, int col, Direction d) {

	grid[row][col].swapWith(d);
	view->draw();

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {

			cerr << grid[i][j].notified << grid[i][j].ready << " ";
		}
		cerr << endl;
	}

	// swap location
	clearSquares(*grid[row][col].neighbour[d]);
	// original location
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

void Board::clearSquares(Square &root) {
	hMatch.clear();
	vMatch.clear();

	int row = root.row;
	int col = root.col;

	// left side
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

	if (hMatch.size() >= 3 && vMatch.size() >= 3) {
		cerr << "L MATCH" << endl;

		for (int i = 0; i < (int)hMatch.size(); i++) {
			cerr << "clearing: (" << hMatch[i]->row << "," << hMatch[i]->col << ")" << endl;
		}

		for (int i = 0; i < (int)vMatch.size(); i++) {
			cerr << "clearing: (" << vMatch[i]->row << "," << vMatch[i]->col << ")" << endl;
		}
	}

	if (hMatch.size() >= 3 && vMatch.size() < 3) {
		cerr << "HORIZONTAL MATCH" << endl;

		if (vMatch.size() == 4) {
			cerr << "LATERAL SQUARE" << endl; 
			for (int i = 0; i < (int)hMatch.size(); i++) {
				cerr << "clearing: (" << hMatch[i]->row << "," << hMatch[i]->col << ")" << endl;

				// set type
				if (i == 1) {
					// if basic type
					hMatch[i]->type = Lateral;
					// else
				} else {
					// if Basic type
					hMatch[i]->colour = Empty;
					// else
				}
			}
		}
	}

	if (hMatch.size() < 3 && vMatch.size() >=3) {
		cerr << "VERTICAL MATCH" << endl;

		if (vMatch.size() == 4) {
			cerr << "UPRIGHT SQUARE" << endl;
			for (int i = 0; i < (int)vMatch.size(); i++) {
				cerr << "clearing: (" << vMatch[i]->row << "," << vMatch[i]->col << ")" << endl;

				// set type
				if (i == 1) {
					// if basic type
					vMatch[i]->type = Upright;
					// else
				} else {
					// if basic type
					vMatch[i]->colour = Empty;
					// else
				}
			}
		}

		if (vMatch.size() == 5) {
			cerr << "PSYCHEDELIC SQUARE" << endl;

			for (int i = 0; i < (int)vMatch.size(); i++) {
				cerr << "clearing: (" << vMatch[i]->row << "," << vMatch[i]->col << ")" << endl;

				if (vMatch[i]->type == Basic) {
					vMatch[i]->setColour(Empty);
					vMatch[i]->notified = false;
					vMatch[i]->ready = false;
				} else if (vMatch[i]->type == Lateral) {

				} else if (vMatch[i]->type == Upright) {

				} else if (vMatch[i]->type == Unstable) {

				} else if (vMatch[i]->type == Psychedelic) {

				}
			}
		}
	}

	if (hMatch.size() < 3 && vMatch.size() < 3) {
		cerr << "NO MATCH" << endl;
	}
}
