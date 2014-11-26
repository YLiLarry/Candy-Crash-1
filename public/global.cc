#include "global.h"
using namespace std;

string dir2str(Direction d) {
    switch (d) {
        case Up: return "up";
        case Down: return "down";
        case Left: return "left";
        case Right: return "right";
    }
}

Colour str2colour(string& str) {
    if (str == "_") {return Empty;}
    if (str == "0") {return White;}
    if (str == "1") {return Red;}
    if (str == "2") {return Green;}
    if (str == "3") {return Blue;}
    return White;
};

Direction str2dir(string& str) {
    if (str == "up") {return Up;}
    if (str == "down") {return Down;}
    if (str == "left") {return Left;}
    if (str == "right") {return Right;}
    return Up;
}

Type str2type(string& str) {
    if (str == "_") {return Basic;}
    if (str == "h") {return Lateral;}
    if (str == "v") {return Upright;}
    if (str == "b") {return Unstable;}
    if (str == "p") {return Psychedelic;}
    return Basic;
}
