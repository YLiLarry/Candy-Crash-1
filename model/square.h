#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "../view/view.h"
#include "../public/global.h"
#include <vector>

class Square {

	int row;
	int col;
	Colour colour;
	Type type;

	int gridSize;

	bool ready;
	bool notified;

	View *view;
	Square **grid;

	void notify();
	void notifyNeighbours();

	public:

	Square *neighbour[4];

	Square();
	void init(int row, int col, int size, Square **grid, View *view);

	void setNeighbours();

	void swap(Direction d);
	void swapWith(Direction d);

	void clearNotifications();

	void clear();

	void drop();
	
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

	void makeReady();

	bool isReady();
	void setReady(bool);

	bool isNotified();

	void printInfo();
};

#endif
