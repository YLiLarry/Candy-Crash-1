#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "cell.h"
#include "../view/view.h"
#include "../public/global.h"
#include <vector>

class Square {
	void notify();

	public:
	int row;
	int col;
	Colour colour;
	Type type;

	Square *neighbour[4];
	View *view;

	bool ready;

	Square();
	void swapWith(Direction d);
	void notifyNeighbours();
	void drop(); // drop if hole below
};

#endif
