#include "global.h"
using namespace std;

string dir2str(Direction d) {
    switch (d) {
        case Up: return "up";
        case Down: return "down";
        case Left: return "left";
        case Right: return "right";
    }
    return "?";
}

Colour str2colour(string& str) {
    if (str == "_") {return Empty;}
    if (str == "0") {return White;}
    if (str == "1") {return Red;}
    if (str == "2") {return Green;}
    if (str == "3") {return Blue;}
    return Empty;
};

Colour char2colour(char& c) {
    if (c == '_') {return Empty;}
    if (c == '0') {return White;}
    if (c == '1') {return Red;}
    if (c == '2') {return Green;}
    if (c == '3') {return Blue;}
    return Empty;
};

char colour2char(Colour c) {
    if (c == Empty) {return '_';}
    if (c == White) {return '0';}
    if (c == Red) {return '1';}
    if (c == Green) {return '2';}
    if (c == Blue) {return '3';}
    return '?';
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

char type2char(Type& t) {
    if (t == Basic) {return '_';}
    if (t == Lateral) {return 'h';}
    if (t == Upright) {return 'v';}
    if (t == Unstable) {return 'b';}
    if (t == Psychedelic) {return 'p';}
    return '?';
}
