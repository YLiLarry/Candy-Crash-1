/*
    This file contains global definitions used by Control, Model and Views,
    for efficient data transfer between them.
*/

#ifndef __GLOBAL_H__    
#define __GLOBAL_H__
#include <string>

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
    
    std::string dir2str(Direction);
    Direction str2dir(std::string& str);
    Colour str2colour(std::string& str);
    Colour char2colour(char& str);
    char colour2char(Colour c);
    Type str2type(std::string& str);
    char type2char(Type& t);
    
namespace Global {
    extern bool TEXTMODE;
    extern bool OPTIMIZED;
    extern int SEED;
    extern std::string SCRIPTFILE;
    extern int STARTLEVEL;
}

#endif
