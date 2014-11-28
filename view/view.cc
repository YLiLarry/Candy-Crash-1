#include "view.h"
#include <unistd.h>
using namespace std;

View:: View(int size) {
    tv = new TextView(size);
    gv = new GraphicView(size);
}

View:: ~View() {
    delete tv;
    delete gv;
}

void View:: init(int size) {    
    tv->init(size);
    gv->init(size);
};

void View:: draw() const {
    tv->draw();
    gv->draw();
};

void View:: setColour(int row, int col, Colour c) {
    tv->setColour(row,col,c);
    gv->setColour(row,col,c);
};

void View:: setType(int row,int col,Type t) {
    tv->setType(row, col, t);
    gv->setType(row, col, t);
};

void View:: setScore(int x) {
    tv->setScore(x);
    gv->setScore(x);
};

void View:: setLevel(int x) {
    tv->setLevel(x);
    gv->setLevel(x);
};

void View:: setLocked(int r, int c, bool x) {
    tv->setLocked(r,c,x);
    gv->setLocked(r,c,x);
};

void View:: setMovesRemain(int x) {
    tv->setMovesRemain(x);
    gv->setMovesRemain(x);
};

void View:: setHiScore(int x) {
    tv->setHiScore(x);
    gv->setHiScore(x);
};

void View:: swap(int r,int c, Direction d) {
    tv->swap(r,c,d);
    gv->swap(r,c,d);
};

void View:: drop(int col, Colour c, Type t) {
    tv->drop(col,c,t);
    gv->drop(col,c,t);
};

void View:: fall(int row, int col) {
    tv->fall(row,col);
    gv->fall(row,col);
};

void View:: destroy(int r,int c) {
    tv->destroy(r,c);
    gv->destroy(r,c);
};

void View:: end() {
    tv->end();
    gv->end();
};

void View:: restart(int size) {
    tv->restart(size);
    gv->restart(size);
};

void View:: print(const string& str) {
    tv->print(str);
    gv->print(str);
}

void View:: setLabel(const string& str) {
    tv->setLabel(str);
    gv->setLabel(str);
}
