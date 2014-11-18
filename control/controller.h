#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "model.h"
#include "player.h"

class Controller {
	Model *model; // may change to be: Grid *grid;
	Player player;

	public:
	void start();
};

#endif
