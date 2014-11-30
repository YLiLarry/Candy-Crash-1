#include "square.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;

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

void Square::init(int r, int c, int n, Square ***g, View *v) {

	row = r;
	col = c;
	gridSize = n;
	grid = g;
	view = v;
}

void Square::setNeighbours() {

	if (row - 1 >=0) {

		neighbour[Up] = grid[row - 1][col];
	}

	if (row + 1 < gridSize) {

		neighbour[Down] = grid[row + 1][col];
	}

	if (col - 1 >= 0) {

		neighbour[Left] = grid[row][col - 1];
	}

	if (col + 1 < gridSize) {

		neighbour[Right] = grid[row][col + 1];
	}
}



void Square::swap(Square &other) {

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

void Square::swap(Direction d) {

	swap(*neighbour[d]);
}

void Square::swapWith(Direction d) {

	swap(*neighbour[d]);
	view->swap(row, col, d);
}

void Square::notify() {

	if (colour == Empty) return;

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

void Square::clearReady() {

	ready = false;

	for (int d = 0; d < NEIGHBOURS; d++) {

		if (neighbour[d] && neighbour[d]->isReady()) {
			neighbour[d]->clearReady();
		}
	}
}

void Square::clearNotified() {

	notified = false;

	for (int d = 0; d < NEIGHBOURS; d++) {

		if (neighbour[d] && neighbour[d]->isNotified()) {
			neighbour[d]->clearNotified();
		}
	}
}

void Square::drop() {
	
	// if (this->colour == Empty) {return;}
	
 //    int i = this->row;
 //    while (i < this->gridSize - 1 && this->grid[i+1][this->col]->colour == Empty) {i++;} // find the destination block
	
	// if (i != this->row) {
		
 //    	Square*& ori = this->grid[this->row][this->col];
 //    	Square*& des = this->grid[i][this->col];
    	
 //    	this->view->fall(this->row, this->col);
    	
 //    	std::swap(ori->row, des->row);
 //    	std::swap(ori->col, des->col);
	// 	std::swap(ori, des);    
    	
 //    }
    
    
	// // inefficient call chain
	// if (! neighbour[Down]) {return;}

	// if (neighbour[Down]->getColour() == Empty) {
		
	// 	neighbour[Down]->setColour(colour);
	// 	neighbour[Down]->setType(type);
		
	// 	setColour(Empty);
	// 	setType(Basic);
		
	// 	// view->fall(row, col); cannot be used here, as will be called `size` times
		
	// 	if (neighbour[Up]) neighbour[Up]->drop();
		
	// } else {
	// 	neighbour[Down]->drop();
	// }
	
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
