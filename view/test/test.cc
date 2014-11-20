#include "../view.h"
#include <iostream>
using namespace std;

Colour str2colour(string& str) {
    if (str == "-1") {return Empty;}
    if (str == "0") {return White;}
    if (str == "1") {return Red;}
    if (str == "2") {return Green;}
    if (str == "3") {return Blue;}
    return White;
};

Direction str2dir(string& str) {
    if (str == "up") {return Up;}
    if (str == "dn") {return Down;}
    if (str == "lt") {return Left;}
    if (str == "rt") {return Right;}
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

int main() {
    View* view = NULL;
    int r, c;
    string str;
    
    while (cin >> str) {
        if (str == "new") {
            cin >> r;
            if (view) {delete view;}
            view = new View(r);
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
