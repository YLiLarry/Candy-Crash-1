#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "cell.h"
#include "../view/view.h"
#include "../public/global.h"
#include <vector>

class Square {
	
	void notify();
	void notifyNeighbours();

	public:

	int row;
	int col;
	Colour colour;
	Type type;

	View *view;


	Square *neighbour[4];

	bool ready;
	bool notified;

	Square();
	void swap(Direction d);
	void swapWith(Direction d);

	void unNotifyNeighbours();

	void drop(); // drop if hole below

	int getRow();
	void setRow(int r);

	int getCol();
	void setCol(int c);

	Colour getColour();
	void setColour(Colour c);

	Type getType();
	void setType(Type t);

	void setNeighbour(Square *neighbour, Direction d);
	void setView(View *v);

#define SQUAREDEBUG
#ifdef SQUAREDEBUG
	void printInfo();
#endif
};

#endif
