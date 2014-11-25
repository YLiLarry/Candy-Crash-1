/*
    This file contains global definitions used by Control, Model and Views,
    for efficient data transfer between them.
*/

#ifndef __GLOBAL_H__    
#define __GLOBAL_H__
#include <cstdlib>

    enum Type { 
		Basic = 0,
		Lateral,
		Upright,
		Unstable, 
		Psychedelic,
	};

    enum Colour { 
		White = 0, 
		Red, 
		Green, 
		Blue, 
		Empty,
	};

    enum Direction { 
		Up = 0, 
		Down, 
		Left, 
		Right,
	};

#endif
