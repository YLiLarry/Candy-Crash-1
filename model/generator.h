#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "../public/global.h"

class Generator {

	static int produced;
	int level;

	public:

	std::string randomSquare(int level);
};

#endif
