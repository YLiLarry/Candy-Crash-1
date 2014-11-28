#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "../public/global.h"

class Generator {

	int level;

	public:

	static int produced;
	std::string randomSquare(int level);
};

#endif
