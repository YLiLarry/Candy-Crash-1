#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "square.h"
#include "../view/view.h"
#include "../public/global.h"

class Board {

	#if DEBUG_BOARD
	    public:
	#endif

	public:
	//View *view;

	Square **grid;
	int size;

	int level;
	int score;
	int cleared;
	int turnScore;

	void setNeighbours(Square *s);

	std::string levelZeroColours;

	// loads level, either randomly generated
	// or from provided file
	void loadLevel(int level);


	std::vector<Square *> hMatch;
	std::vector<Square *> vMatch;



	// scans the grid for matches
	// shouldint row, int col be used to detect chain reactions
	// after swap, not for "swap matches"
	//
	// append matched vector with all matches
	// and call clearSquares(), which in turn
	// calls checkMatch() again. Will stop
	// when there are no matches (matched is empty)
	void chainReaction();

	std::string validMove();

	bool hasMove();


	//void getMatches(Square &root);
	void collectMatched(Square &root);

	void clearRow(int row);
	void clearCol(int col);
	void clearRad(int row, int col, int rad);
	void clearColour(Colour c);

	//void clear(Square &root);
	//int clearSquares(Square &root);
	void clearSquares(Square &root);

	void setNewSquare(Square &sq);

	public:

	Board(int size, View *view);
	~Board();

	void start(int level);
	void swap(int row, int col, Direction d);

	void clearAt(int row, int col);
	void clearNotifications(int row, int col);
	void dropSquares();

	std::string hint();

	void scramble();

	void levelUp();
	void levelDown();

	void restart();


	// development
	void printGridInfo();
};

#endif
