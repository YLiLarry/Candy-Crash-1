#ifndef __GAME_H__
#define __GAME_H__

#include "../model/board.h"

class Game {
	Board *board;

	public:
	// processes player commands
	void startGame();
};

#endif
