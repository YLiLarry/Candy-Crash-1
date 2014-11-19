#include "textview/textview.h"
#include <iostream>
using namespace std;

COLOUR str2colour(string& str) {
    if (str == "-1") {return EMPTY;}
    if (str == "0") {return WHITE;}
    if (str == "1") {return RED;}
    if (str == "2") {return GREEN;}
    if (str == "3") {return BLUE;}
    return WHITE;
};

DIRECTION str2dir(string& str) {
    if (str == "up") {return UP;}
    if (str == "dn") {return DOWN;}
    if (str == "lt") {return LEFT;}
    if (str == "rt") {return RIGHT;}
    return UP;
}

CELLTYPE str2type(string& str) {
    if (str == "bc") {return BASIC;}
    if (str == "ll") {return LATERAL;}
    if (str == "ut") {return UPRIGHT;}
    if (str == "ue") {return UNSTABLE;}
    if (str == "pc") {return PSYCHEDELIC;}
    return BASIC;
}

int main() {
    View* view = new TextView(5);
    int r, c;
    string str;
    
    while (cin >> str) {
        if (str == "//") {
            cin.ignore(1024, '\n');
            continue;
        } else
        if (str == "dw") {
            view->draw();
        } else 
        if (str == "sp") {
            cin >> r >> c >> str;
            view->swap(r, c, str2dir(str));
        } else
        if (str == "sc") {
            cin >> r >> c >> str;
            view->setColour(r, c, str2colour(str));
        } else
        if (str == "st") {
            cin >> r >> c >> str;
            view->setType(r, c, str2type(str));
        } else
        if (str == "ss") {
            cin >> r;
            view->setScore(r);
        } else 
        if (str == "sl") {
            cin >> r;
            view->setLevel(r);
        } else
        if (str == "sm") {
            cin >> r;
            view->setMovesRemain(r);
        } else 
        if (str == "sh") {
            cin >> r;
            view->setHiScore(r);
        } else 
        if (str == "dy") {
            cin >> r >> c;
            view->destory(r, c);
        } if (str == "quit") {
            break;
        }
    }
    return 0;
}
