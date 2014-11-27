#ifndef __BOARD_H__
#define __BOARD_H__

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "generator.h"
#include "square.h"
#include "../view/view.h"
#include "../public/global.h"

class Board {

	/*
	 *#if DEBUG_BOARD
	 *    public:
	 *#endif
	 */

	View *view;

	Generator *generate;
	Square **grid;
	int size;

	int cleared;
	int chain;
	unsigned int score;
	unsigned int initScore;
	unsigned int matchScore;
	unsigned int turnScore;
	
	bool chainMode;
	bool settled;

	void setNeighbours(Square *s);

	std::string levelZeroColours;

	// loads level, either randomly generated
	// or from provided file


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
	void clearAt(Square &root);
	void clear(Square &sq, int radius);

	void dropSquares();
	void setNewSquare(Square &sq);


	void chainReaction();

	public:

	int level;

	Board(int size);
	~Board();

	void start();
	void swap(int row, int col, Direction d);

	void scramble();

	void levelUp();
	void levelDown();
	void loadLevel(int level);

	void restart();

	void hint();

	// development
	void printGridInfo();
};

#endif
