#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "cell.h"
#include "../public/global.h"

class Square {
	public:

	int row;
	int col;
	Colour colour;
	Type type;

	void drop(); // drop if hole below
};

#endif
