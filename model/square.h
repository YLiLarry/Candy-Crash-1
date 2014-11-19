#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "cell.h"
class Square {
	public:
	int x;
	int y;
	COLOR color; // let's keep the spelling consistent
	CELLTYPE type;

	void drop(); // drop if hole below
};

#endif
