#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "cell.h"
#include "../enums.h"
class Square {
	public:
	int x;
	int y;
	Colour colour;
	CellType type;

	void drop(); // drop if hole below
};

#endif
