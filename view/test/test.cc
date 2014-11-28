#include "../view.h"
#include "../../public/global.h"
#include <iostream>
using namespace std;


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
        if (str == "fall") {
            cin >> r >> c;
            view->fall(r, c);
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
            view->destroy(r, c);
        } else 
        if (str == "qt") {
            break;
        }
    }
    
    delete view;
    return 0;
}
