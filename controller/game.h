#ifndef __GAME_H__
#define __GAME_H__

#include "../model/board.h"
#include "../public/global.h"

class Game {
    
    #if DEBUG
        public:
    #endif
        
    Board *board;

	public:
	// processes player commands
    Game(int);
    void restart();
    void hint();
    void scramble();
    void swap();
    void levelUp();
    void levelDown();
    ~Game();
};

#endif
