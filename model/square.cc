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

void Square::swap(Direction d) {
	Colour tColour = colour;
	Type tType = type;

	colour = neighbour[d]->colour;
	type = neighbour[d]->type;

	neighbour[d]->colour =tColour;
	neighbour[d]->type = tType;

	view->swap(row, col, d);
}

void Square::swapWith(Direction d) {

	swap(d);

	neighbour[d]->notify();
	notify();
}

void Square::notify() {

	//cerr << "I have been notified (" << row << "," << col << ")" << endl;

	//printInfo();

	notified = true;

	// horizontal matching
	if (neighbour[Left] && neighbour[Right]) {
		if (neighbour[Left]->colour == colour &&
			neighbour[Right]->colour == colour) {

			cerr << "a horizontal match" << endl;

			this->ready = true;
			neighbour[Left]->ready = true;
			neighbour[Right]->ready = true;
		}
	}

	// vertical matching
	if (neighbour[Up] && neighbour[Down]) {
		if (neighbour[Up]->colour == colour &&
			neighbour[Down]->colour == colour) {

			cerr << "a vertical match" << endl;
			ready = true;
			neighbour[Up]->ready = true;
			neighbour[Down]->ready = true;
		}
	}

	notifyNeighbours();
}

void Square::notifyNeighbours() {
	for (int i = 0; i < 4; i++) {
		if (neighbour[i] && // not null
			neighbour[i]->colour == this->colour && // same colour
			neighbour[i]->colour != Empty && // not empty
			neighbour[i]->notified == false) { // not notified 
			neighbour[i]->notify();
		}
	}
}

void Square::unNotifyNeighbours() {
	//cerr << "unnotifying (" << row << "," << col << ")" << endl;
	this->notified = false;

	for (int i = 0; i < 4; i++) {
		if (neighbour[i] && neighbour[i]->notified) {
			neighbour[i]->unNotifyNeighbours();
		}
	}
}

int Square::getRow() {
	return this->row;
}
void Square::setRow(int r) {
	this->row = r;
}

int Square::getCol() {
	return this->col;
}

void Square::setCol(int c) {
	this->col = c;
}

Colour Square::getColour() {
	return this->colour;
}

void Square::setColour(Colour c) {
	this->colour = c;
	view->setColour(this->row, this->col, this->colour);
}

Type Square::getType() {
	return this->type;
}

void Square::setType(Type t) {
	this->type = t;
	view->setType(this->row, this->col, this->type);
}

void Square::setNeighbour(Square *neighbour, Direction d) {
	this->neighbour[d] = neighbour;
}

void Square::setView(View *v) {
	this->view = v;
}



void Square::printInfo() {
	cerr << "---- Square (" << this->row << "," << this->col << ") ----" << endl;
	cerr << "colour  : " << colour << endl;
	cerr << "type    : " << type << endl;
	cerr << "ready   : " << ready << endl;
	cerr << "notified: " << notified << endl;
	cerr << "----------------------" << endl;
}
