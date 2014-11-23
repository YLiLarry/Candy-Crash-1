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
	int row = this->row;
	int col = this->col;
	Colour colour = this->colour;
	Type type = this->type;

	this->row = neighbour[d]->row;
	this->col = neighbour[d]->col;
	this->colour = neighbour[d]->colour;
	this->type = neighbour[d]->type;

	neighbour[d]->row = row;
	neighbour[d]->col = col;
	neighbour[d]->colour = colour;
	neighbour[d]->type = type;

	view->swap(row, col, d);
	
	notify();
}

void Square::notify() {
	// horizontal matching
	if (neighbour[Left]->colour == colour &&
		neighbour[Right]->colour == colour) {

		ready = true;
		neighbour[Left]->ready = true;
		neighbour[Right]->ready = true;
	}

	// vertical matching
}
