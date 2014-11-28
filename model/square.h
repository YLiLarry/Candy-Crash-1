#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "../view/view.h"
#include "../public/global.h"
#include <vector>

const int NEIGHBOURS = 4;

class Square {

	int row;
	int col;

	bool locked;
	Colour colour;
	Type type;

	int gridSize;

	bool ready;
	bool notified;

	View *view;
	Square **grid;

	void notifyNeighbours();

	public:

	Square *neighbour[NEIGHBOURS];

	Square();
	void init(int row, int col, int size, Square **grid, View *view);

	void setNeighbours();

	void swap(Direction d);
	void swap(Square &sq);
	void swapWith(Direction d);

	void notify();
	
	void clearReady();
	void clearNotified();

	//void clear(int &clear, int &chain, int &turnScore, int radius);

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
