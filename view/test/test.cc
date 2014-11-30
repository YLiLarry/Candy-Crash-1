#include "../view.h"
#include "../../public/global.h"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;


int main() {
    View* view = new View(10);
    int r, c;
    string str;
    
    // view->setColour(4,3,Red);
    // view->drop(3,Red,Basic);
    // view->drop(3,Red,Basic);
    // view->drop(3,Red,Basic);
    // view->drop(3,Red,Basic);
    // view->drop(3,Red,Basic);
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
            delete view;
            view = new View(10);
            view->draw();
        } else
        if (str == "//") {
            cin.ignore(1024, '\n');
            continue;
        } else
        if (str == "draw") {
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
        if (str == "setColour") {
            cin >> r >> c >> str;
            view->setColour(r, c, str2colour(str));
            view->draw();
        } else
        if (str == "setType") {
            cin >> r >> c >> str;
            view->setType(r, c, str2type(str));
        } else
        if (str == "setScore") {
            cin >> r;
            view->setScore(r);
        } else 
        if (str == "setLevel") {
            cin >> r;
            view->setLevel(r);
        } else
        if (str == "setMovesRemain") {
            cin >> r;
            view->setMovesRemain(r);
        } else 
        if (str == "setHiScore") {
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
        if (str == "destroy") {
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
