#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "cell.h"
#include "../view/view.h"
#include "../public/global.h"
#include <vector>

class Square {
	void notify();
	void notify(Direction d);

	public:

	int row;
	int col;
	Colour colour;
	Type type;
	Square *neighbours[4];

	Square *neighbour[4];
	View *view;

	bool ready;

	bool notified;

	Square();
	void swapWith(Direction d);
	void notifyNeighbours();
	void drop(); // drop if hole below

	void setColour(Colour c);
	void setType(Type t);

#define SQUAREDEBUG
#ifdef SQUAREDEBUG
	void printInfo();
#endif
};

#endif
