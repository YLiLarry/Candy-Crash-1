#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <vector>
#include "square.h"
#include "model.h"
#include "../view/view.h"

class Board {
	// pointer to View abstract class
	View *view;

	// the grid of squares
	// note: turns out it's better to make 2d-arrays this way
	Square **grid;

	// current level
	int level;

	// player score
	// determines when to auto level up/down
	int score;

	// players moves remaining
	int movesRemain;

	// matched squares
	std::vector<Square> matched;

	/*
	 * methods
	 */

	// appends matched squares to matched vector
	// should be used after swap
	void hMatch(int x, int y);
	void vMatch(int x, int y);

	// clears squares stored in matched vector
	// and empties matched vector after
	// also determines where to place special square
	// returns the score obtained in that turn
	//
	// note: clearSquares() will call clear(int x, int y)
	// to handle clearing with a special square
	int clearSquares();

	// clears squares at x, y coordinates
	// should be used for speacial clearing
	// e.g. clear entire row/col, 5x5 hole
	void clear(int x, int y);

	// scans the grid for matches
	// should be used to detect chain reactions
	// after swap, not for "swap matches"
	//
	// append matched vector with all matches
	// and call clearSquares(), which in turn
	// calls checkMatch() again. Will stop
	// when there are no matches (matched is empty)
	void chainReaction();

	public:
	Board();
	~Board();
	void swap(int x, int y, DIRECTION d);
	void hint();
	void scramble();
	void levelUp();
	void levelDown();
	void restart();
	friend std::ostream &operator<<(std::ostream &out, const Board &board);
};

#endif
