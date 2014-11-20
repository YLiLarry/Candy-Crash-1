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

#ifdef DEBUG
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cerr << grid[i][j].colour << " ";
		}
		cerr << endl;
	}
#endif
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

		ifstream file("sequence.txt");

		string square;

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {

				file >> square;

				// not used at the moment
				//int advanced;
				//advanced = (square[0] == '_') ? 0 : (square[0] - '0');

				Type type;

				switch (square[1]) {
					case '_': type = Basic; break;
					case 'h': type = Lateral; break;
					case 'v': type = Upright; break;
					case 'b': type = Unstable; break;
					case 'p': type = Psychedelic; break;
				}

				Colour colour;
				colour = (Colour)(square[2] - '0');

				grid[i][j].row = i;
				grid[i][j].col = j;
				grid[i][j].colour = colour;
				grid[i][j].type = type;

				view->setColour(i, j, colour);
				view->setType(i, j, type);
				view->setScore(score);
				view->setLevel(level);
			}
		}
	}
}

void swapWith(Square &a, Square &b) {
#ifdef DEBUG
	cerr << "square a: " << endl;
	cerr << "row: " << a.row << " col: "<< a.col << endl;
	cerr << "colour: " << a.colour << " type: " << a.type << endl;

	cerr << endl << "square b: " << endl;
	cerr << "row: " << b.row << " col: "<< b.col << endl;
	cerr << "colour: " << b.colour << " type: " << b.type << endl;
#endif

	int tRow = a.row;
	int tCol = a.col;
	Colour tColour = a.colour;
	Type tType = a.type;

	a.row = b.row;
	a.col = b.col;
	a.colour = b.colour;
	a.type = b.type;

	b.row = tRow;
	b.col = tCol;
	b.colour = tColour;
	b.type = tType;

#ifdef DEBUG
	cerr << endl;

	cerr << "square a: " << endl;
	cerr << "row: " << a.row << " col: "<< a.col << endl;
	cerr << "colour: " << a.colour << " type: " << a.type << endl;

	cerr << endl << "square b: " << endl;
	cerr << "row: " << b.row << " col: "<< b.col << endl;
	cerr << "colour: " << b.colour << " type: " << b.type << endl;
#endif
}

void Board::swap(int row, int col, Direction d) {

	vector<Square *> matched;
	switch (d) {
		case Up: swapWith(grid[row][col], grid[row - 1][col]);
				 matched = findMatches(row - 1, col);
				 break;
		case Down: swapWith(grid[row][col], grid[row + 1][col]);
				   matched = findMatches(row + 1, col);
				   break;
		case Left: swapWith(grid[row][col], grid[row][col - 1]);
				   matched = findMatches(row, col - 1);
				   break;
		case Right: swapWith(grid[row][col], grid[row][col + 1]);
					matched = findMatches(row, col + 1);
					break;
	}

	view->swap(row, col, d);
	view->draw();

	// test

	cerr << "----matched---" << endl;
	for (int i = 0; i < (int)matched.size(); i++) {
		//cerr << "row: " << matched[i]->row 
			//<< " col: " << matched[i]->col << endl;
		cerr << "colour: " << matched[i]->colour << endl;
		//cerr << "type: " << matched[i]->colour << endl;
	}

#ifdef DEBUG
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cerr << grid[i][j].colour << " ";
		}
		cerr << endl;
	}
#endif

}

vector<Square *> Board::findMatches(int row, int col) {
	vector<Square *> matched;

	Colour colour = grid[row][col].colour;
	//Type type = swapped->type;

	// check horizontally for matching set of three
	for (int c = 0; c < size; c++) {
		if (c + 1 < size && c + 2 < size) {
			if (grid[row][c].colour == colour &&
					grid[row][c + 1].colour == colour &&
					grid[row][c + 2].colour == colour) {

				matched.push_back(&grid[row][c]);
				matched.push_back(&grid[row][c + 1]);
				matched.push_back(&grid[row][c + 2]);

				// add the rest (if any to matched vector)
				c++;
				while (grid[row][c].colour == colour) {
					matched.push_back(&grid[row][c]);
					c++;
				}
			}
		}
	}

	// check vertically for matching set of three
	for (int r = 0; r < size; r++) {
		if (r + 1 < size && r + 2 < size) {
			if (grid[r][col].colour == colour &&
					grid[r + 1][col].colour == colour &&
					grid[r + 2][col].colour == colour) {

				matched.push_back(&grid[r][col]);
				matched.push_back(&grid[r + 1][col]);
				matched.push_back(&grid[r + 2][col]);

				r++;
				while (grid[r][col].colour == colour) {
					matched.push_back(&grid[r][col]);
					r++;
				}
			}
		}
	}

	return matched;
}
