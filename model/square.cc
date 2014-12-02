#include "square.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;

//
// Square default constructor
//
Square::Square() {

	row = 0;
	col = 0;
	locked = false;
	colour = Empty;
	type = Basic;

	gridSize = 0;

	ready = false;
	notified = false;

	neighbour[Up] = NULL;
	neighbour[Down] = NULL;
	neighbour[Left] = NULL;
	neighbour[Right] = NULL;

	grid = NULL;
	view = NULL;
}

//
// Initializes the square with rudimentary information
//
void Square::init(int r, int c, int n, Square ***g, View *v) {

	row = r;
	col = c;
	gridSize = n;
	grid = g;
	view = v;
}

//
// The square adds the neighbours to their appropriate slot.
//
void Square::setNeighbours() {

	neighbour[Up] = (row - 1 >= 0)? grid[row - 1][col] : NULL;
	neighbour[Down] = (row + 1 < gridSize)? grid[row + 1][col] : NULL;
	neighbour[Left] = (col - 1 >= 0)? grid[row][col - 1] : NULL;
	neighbour[Right] = (col + 1 < gridSize)? grid[row][col + 1] : NULL;
}

//
// Swap colour and type with another square.
// Does notify the view.
//
void Square::swap(Square &other) {

	int tRow = row;
	int tCol = col;
	bool tLocked = locked;
	Colour tColour = colour;
	Type tType = type;

	locked = other.locked;
	colour = other.colour;
	type = other.type;

	other.locked = tLocked;
	other.colour = tColour;
	other.type = tType;

	other.notify();
	notify();
}

//
// Swap colour and type with neighbour in specified direction.
// Does not notify the view.
//
void Square::swap(Direction d) {

	swap(*neighbour[d]);
}

//
// Swap colour and type with neighbour in specified direction.
// Notifies the view.
//
void Square::swapWith(Direction d) {

	swap(*neighbour[d]);
	view->swap(row, col, d);
}

//
// Notify the neighbours in horizontal and vertical directions.
// If and only if they exist and match with selfs colour.
// This square becomes 'notified'.
//
void Square::notify() {

	if (colour == Empty) return;

	notified = true;

	#if DEBUG_SQUARE
		if (row == 7 && col == 5) {
			    fprintf(stderr,"",);
				printInfo();
				neighbour[Left]->printInfo();
				neighbour[Right]->printInfo();
		}
	#endif

	if (neighbour[Left] && neighbour[Right]) {
		if (neighbour[Left]->colour == colour &&
			neighbour[Right]->colour == colour) {

			this->ready = true;
			neighbour[Left]->ready = true;
			neighbour[Right]->ready = true;
		}
	}

	if (neighbour[Up] && neighbour[Down]) {
		if (neighbour[Up]->colour == colour &&
			neighbour[Down]->colour == colour) {

			this->ready = true;
			neighbour[Up]->ready = true;
			neighbour[Down]->ready = true;
		}
	}

	notifyNeighbours();
}

//
// Notify all neighbours, if they exist, is not notified,
// and has a colour which matches with self.
//
void Square::notifyNeighbours() {

	for (int d = 0; d < 4; d++) {
		if (neighbour[d] &&
			neighbour[d]->notified == false &&
			neighbour[d]->colour != Empty &&
			neighbour[d]->colour == this->colour) {

			neighbour[d]->notify();
		}
	}
}

//
// If a neighbour exists, and is ready to be cleared,
// remove its eagerness to be cleared.
//
void Square::clearReady() {

	ready = false;

	for (int d = 0; d < NEIGHBOURS; d++) {

		if (neighbour[d] && neighbour[d]->isReady()) {
			neighbour[d]->clearReady();
		}
	}
}

//
// If a neighbour exists, and is notified,
// make it forget that it was.
//
void Square::clearNotified() {

	notified = false;

	for (int d = 0; d < NEIGHBOURS; d++) {

		if (neighbour[d] && neighbour[d]->isNotified()) {
			neighbour[d]->clearNotified();
		}
	}
}

int Square::getRow() { return row; }
void Square::setRow(int r) { row = r; }

int Square::getCol() { return col; }
void Square::setCol(int c) { col = c; }

bool Square::isLocked() { return locked; }
void Square::setLocked(bool t) { locked = t; }

Colour Square::getColour() { return colour; }
void Square::setColour(Colour c) { colour = c; }

Type Square::getType() { return type;  }
void Square::setType(Type t) { type = t; }

bool Square::isReady() { return ready; }
void Square::setReady(bool t) { ready = t; }

bool Square::isNotified() { return notified; }
void Square::setNotified(bool t) { notified = t; }

void Square::printInfo() {
	cerr << "---- Square (" << this->row << "," << this->col << ") ----" << endl;
	cerr << "colour  : " << colour << endl;
	cerr << "type    : " << type << endl;
	cerr << "ready   : " << ready << endl;
	cerr << "notified: " << notified << endl;
	cerr << "----------------------" << endl;
}
