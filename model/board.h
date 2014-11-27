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

	View *view;

	Square **grid;
	int size;

	int level;
	int score;
	int cleared;
	int turnScore;
	int chain;
	
	bool chainMode;
	bool settled;

	void setNeighbours(Square *s);

	std::string levelZeroColours;

	// loads level, either randomly generated
	// or from provided file
	void loadLevel(int level);


	std::vector<Square *> hMatch;
	std::vector<Square *> vMatch;

	std::string validMove();

	bool hasMove();


	//void getMatches(Square &root);
	void collectMatched(Square &root);

	void clearRow(int row);
	void clearCol(int col);
	void clearRad(int row, int col, int rad);
	void clearColour(Colour c);

	//void clear(Square &root);
	int clearAt(Square &root);
	void clear(Square &sq, int radius);

	void dropSquares();
	void setNewSquare(Square &sq);


	void chainReaction();

	public:

	Board(int size);
	~Board();

	void start();
	void swap(int row, int col, Direction d);

	void scramble();

	void levelUp();
	void levelDown();

	void restart();

	void hint();

	// development
	void printGridInfo();
};

#endif
