#include "square.h"
#include <iostream>
using namespace std;

Square::Square() {
	row = 0;
	col = 0;
	colour = Empty;
	type = Basic;

	neighbour[Up] = NULL;
	neighbour[Down] = NULL;
	neighbour[Left] = NULL;
	neighbour[Right] = NULL;

	view = NULL;
}

void Square::swapWith(Direction d) {

	Colour tColour = colour;
	Type tType = type;

	colour = neighbour[d]->colour;
	type = neighbour[d]->type;

	neighbour[d]->colour =tColour;
	neighbour[d]->type = tType;

	neighbour[d]->notify();
	notify();

	view->swap(row, col, d);
}

void Square::notify() {

	cerr << "I have been notified (" << row << "," << col << ")" << endl;

	notified = true;

	// horizontal matching
	if (neighbour[Left] && neighbour[Right]) {
		if (neighbour[Left]->colour == colour &&
			neighbour[Right]->colour == colour) {

			ready = true;
			neighbour[Left]->ready = true;
			neighbour[Right]->ready = true;
		}
	}

	// vertical matching
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
	for (int i = 0; i < 4; i++) {
		if (neighbour[i] && neighbour[i]->colour == colour && !neighbour[i]->notified) {
			neighbour[i]->notify();
		}
	}
}

void Square::printInfo() {
	cerr << "---- Square (" << this->row << "," << this->col << ") ----" << endl;
	cerr << "colour  : " << colour << endl;
	cerr << "type    : " << type << endl;
	cerr << "ready   : " << ready << endl;
	cerr << "notified: " << notified << endl;
	cerr << "----------------------" << endl;
}
