#include "../view.h"
#include "../../public/global.h"
#include <iostream>
using namespace std;


int main() {
    View* view = new View(5);
    int r, c;
    string str;
    
    view->drop(3,Red,Basic);
    view->drop(3,Red,Basic);
    view->drop(3,Red,Basic);
    view->drop(3,Red,Basic);
    view->drop(3,Red,Basic);
    // view->destroy(4,3);
    // view->destroy(3,3);
    // view->destroy(2,3);
    // view->destroy(1,3);
    // view->destroy(0,3);
    // view->drop(3,Blue,Basic);
    // view->drop(3,Blue,Basic);
    // view->drop(3,Blue,Basic);
    // view->drop(3,Blue,Basic);
    // view->drop(3,Blue,Basic);
    // view->destroy(4,3);
    // view->destroy(3,3);
    // view->destroy(2,3);
    // view->destroy(1,3);
    // view->destroy(0,3);
    
    // view->swap(0,0,Down);
    // view->swap(0,0,Down);
    
    while (cin >> str) {
        if (str == "new") {
            if (view) {delete view;}
            view = new View(5);
            view->draw();
        } else
        if (str == "//") {
            cin.ignore(1024, '\n');
            continue;
        } else
        if (str == "dw") {
            view->draw();
        } else 
        if (str == "swap") {
            cin >> r >> c >> str;
            view->swap(r, c, str2dir(str));
            view->draw();
        } else
        if (str == "fall") {
            cin >> r >> c;
            view->fall(r, c);
        } else
        if (str == "sc") {
            cin >> r >> c >> str;
            view->setColour(r, c, str2colour(str));
            view->draw();
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
        if (str == "drop") {
            string str1, str2;
            cin >> c >> str1 >> str2;
            view->drop(c, str2colour(str1), str2type(str2));
        } else 
        if (str == "setLabel") {
            string str1;
            cin >> str1;
            view->setLabel(str1);
        } else 
        if (str == "dy") {
            cin >> r >> c;
            view->destroy(r, c);
        } else 
        if (str == "quit") {
            break;
        }
    }
    
    delete view;
    return 0;
}
