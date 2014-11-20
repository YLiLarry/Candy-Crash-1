/*
    This file contains global definitions used by Control, Model and Views,
    for efficient data transfer between them.
*/
        
#ifndef __GLOBAL_H__    
#define __GLOBAL_H__

    enum Type { 
		Basic = 0,
		Lateral,
		Upright,
		Unstable, 
		Psychedelic 
	};

    enum Colour { 
		White = 0, 
		Blue, 
		Green, 
		Red, 
		Empty 
	};

    enum Direction { 
		Up = 0, 
		Down, 
		Left, 
		Right 
	};

#endif
