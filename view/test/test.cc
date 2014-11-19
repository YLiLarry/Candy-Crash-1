#include "../textview/textview.h"
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
    if (str == "_") {return BASIC;}
    if (str == "h") {return LATERAL;}
    if (str == "v") {return UPRIGHT;}
    if (str == "b") {return UNSTABLE;}
    if (str == "p") {return PSYCHEDELIC;}
    return BASIC;
}

int main() {
    View* view = NULL;
    int r, c;
    string str;
    
    while (cin >> str) {
        if (str == "new") {
            cin >> r;
            if (view) {delete view;}
            view = new TextView(r);
        } else
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
        if (str == "dp") {
            string str1, str2;
            cin >> c >> str1 >> str2;
            view->drop(c, str2colour(str1), str2type(str2));
        } else 
        if (str == "dy") {
            cin >> r >> c;
            view->destory(r, c);
        } else 
        if (str == "qt") {
            break;
        }
    }
    
    delete view;
    return 0;
}
