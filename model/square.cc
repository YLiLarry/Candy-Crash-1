#include "square.h"
#include <iostream>
using namespace std;

Square::Square() {

	row = 0;
	col = 0;
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

void Square::init(int r, int c, int n, Square **g, View *v) {

	row = r;
	col = c;
	gridSize = n;
	grid = g;
	view = v;
}

void Square::setNeighbours() {

	if (row - 1 >=0) {

		neighbour[Up] = &grid[row - 1][col];
	}

	if (row + 1 < gridSize) {

		neighbour[Down] = &grid[row + 1][col];
	}

	if (col - 1 >= 0) {

		neighbour[Left] = &grid[row][col - 1];
	}

	if (col + 1 < gridSize) {

		neighbour[Right] = &grid[row][col + 1];
	}
}

void Square::swap(Direction d) {

	Colour tColour = colour;
	Type tType = type;

	colour = neighbour[d]->getColour();
	type = neighbour[d]->getType();

	neighbour[d]->setColour(tColour);
	neighbour[d]->setType(tType);

	neighbour[d]->notify();
	notify();
}

void Square::swapWith(Direction d) {

	swap(d);
	view->swap(row, col, d);
}

void Square::notify() {

	notified = true;

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

			ready = true;
			neighbour[Up]->ready = true;
			neighbour[Down]->ready = true;
		}
	}

	notifyNeighbours();
}

void Square::notifyNeighbours() {

	for (int d = 0; d < 4; d++) {
		if (neighbour[d] &&
				neighbour[d]->colour == this->colour &&
				neighbour[d]->colour != Empty &&
				neighbour[d]->notified == false) {

			neighbour[d]->notify();
		}
	}
}

void Square::clearNotifications() {

	notified = false;

	for (int i = 0; i < 4; i++) {
		if (neighbour[i] && neighbour[i]->isNotified()) {

			neighbour[i]->clearNotifications();
		}
	}
}

int Square::getRow() { return row; }
void Square::setRow(int r) { row = r; }

int Square::getCol() { return col; }
void Square::setCol(int c) { col = c; }

Colour Square::getColour() { return colour; }
void Square::setColour(Colour c) {
	colour = c;
	view->setColour(row, col, colour);
}

Type Square::getType() { return type;  }
void Square::setType(Type t) {
	type = t;
	view->setType(row, col, type);
}

bool Square::isReady() { return ready; }
void Square::setReady(bool t) { ready = t; }

bool Square::isNotified() { return notified; }

void Square::printInfo() {
	cerr << "---- Square (" << this->row << "," << this->col << ") ----" << endl;
	cerr << "colour  : " << colour << endl;
	cerr << "type    : " << type << endl;
	cerr << "ready   : " << ready << endl;
	cerr << "notified: " << notified << endl;
	cerr << "----------------------" << endl;
}