#include <fstream>
#include <sstream>
#include "board.h"
#include "../view/textview/textview.h"
#include "../public/global.h"

using namespace std;

Board::Board(int n) : size(n) {

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

// helper. swaps member values of a and b
void _swap(Square &a, Square &b) {
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
}

void Board::swapMechanism(int row, int col, Direction d) {

	// safe guard against invalid swaps
	if ((row == 0 && d == Up) ||
		(col == 0 && d == Left) ||
		(row == size - 1 && d == Down) ||
		(col == size - 1 && d == Right)) {

		return;
	}

	Square a = grid[row][col], b;

	switch (d) {
		case Up: b = grid[row - 1][col]; break;
		case Down: b = grid[row + 1][col]; break;
		case Left: b = grid[row][col - 1]; break;
		case Right: b = grid[row][col + 1]; break;
	}

	_swap(a, b);
}

void Board::swap(int row, int col, Direction d) {

	swapMechanism(row, col, d);

	view->swap(row, col, d);
	view->draw();
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
            b.swapMechanism(i,j,Up);
            if (! b.findMatches(i,j).empty()) {
            	ss << i << " " << j << " " << (int)Up;
            	return ss.str();
            };
            b.swapMechanism(i,j,Up);
            b.swapMechanism(i,j,Down);
            if (! b.findMatches(i,j).empty()) {
            	ss << i << " " << j << " " << (int)Down;
            	return ss.str();
            };
            b.swapMechanism(i,j,Down);
            b.swapMechanism(i,j,Left);
            if (! b.findMatches(i,j).empty()) {
            	ss << i << " " << j << " " << (int)Left;
            	return ss.str();
            };
            b.swapMechanism(i,j,Left);
            b.swapMechanism(i,j,Right);
            if (! b.findMatches(i,j).empty()) {
            	ss << i << " " << j << " " << (int)Right;
            	return ss.str();
            };
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
			view->setColour(i, j, grid[i][j].colour);
			view->setType(i, j, grid[i][j].type);
		}
	}

	view->draw();
}
