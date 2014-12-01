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
	
	View *view;
	Generator *generate;
	Square ***grid;
	int size;

	int cleared;
	int chain;
	int unlocked;

	unsigned int score;
	unsigned int initScore;
	unsigned int matchScore;
	unsigned int turnScore;
	
	bool chainMode;
	bool emptyBoard;

	std::string levelZeroColours;

	std::vector<Square *> hMatch;
	std::vector<Square *> vMatch;


	void checkLevel();

	void collectMatched(Square *root);

	void clearAt(Square *root);
	void clear(Square* sq, int radius);

	void setNewSquare(Square* sq);
	void setNewSquare(Square* sq, std::string strSquare);

	void dropSquares();
	void chainReaction();
	
	std::string validMove();

	public:
	void notifyAll();
	void unNotifyAll();

	//public:

	int level;

	Board(int size);
	~Board();

	void swap(int row, int col, Direction d);
	void scramble(bool force = false);
	void levelUp();
	void levelDown();
	void loadLevel(int level);
	void restart();
	void hint();
	// development
	void printGridInfo();
};

#endif
