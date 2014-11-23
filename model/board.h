#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <vector>
#include "square.h"
#include "model.h"
#include "../view/view.h"
#include "../public/global.h"

class Board {
	// pointer to views
	View *view;

	// the grid of squares
	// note: turns out it's better to make 2d-arrays this way
	Square **grid;

	// size of the grid
	int size;

	// current level
	int level;

	// player score
	// determines when to auto level up/down
	int score;

	// players moves remaining
	int movesRemain;

	// loads level, either randomly generated
	// or from provided file
	void loadLevel(int level);

	std::vector<Square *> hMatch;
	std::vector<Square *> vMatch;

	// takes in the coordinates of a square anc checks
	// if it is matching set, if it is then findMatches
	// returns a vector of squares that should be cleared
	std::vector<Square *> findMatches(int row, int col);
	
	// clears squares stored in matched vector
	// and empties matched vector after
	// also determines where to place special square
	// returns the score obtained in that turn
	//
	// note: clearSquares() will call clear(int x, int y)
	// to handle clearing with a special square
	// that requires to clear squares NOT in matched
	int clearSquares(std::vector<Square *> matched);

	// clears squares at x, y coordinates
	// should be used for speacial clearing
	// e.g. clear entire row/col, 5x5 hole
	void clear(int x, int y);

	// scans the grid for matches
	// shouldint row, int col be used to detect chain reactions
	// after swap, not for "swap matches"
	//
	// append matched vector with all matches
	// and call clearSquares(), which in turn
	// calls checkMatch() again. Will stop
	// when there are no matches (matched is empty)
	void chainReaction();

	void clearSquares(Square &root, Direction d);
	void clearSquares(Square &root);
	void getMatches(Square &root);
	public:
	Board(int n);
	~Board();
	void swap(int row, int col, Direction d);
	void hint();
	void scramble();
	void levelUp();
	void levelDown();
	void restart();
	friend std::ostream &operator<<(std::ostream &out, const Board &board);
};

#endif
