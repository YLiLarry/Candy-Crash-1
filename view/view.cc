#include "view.h"
View:: View(int size) {
    tv = new TextView(size);
    // gv = new GraphicView(size);
}

View:: ~View() {
    delete tv;
    // delete gv;
}

void View:: init(int size) {    
    tv->init(size);
    // gv->init(size);
};

void View:: draw() const {
    tv->draw();
    // gv->draw();
};

void View:: setColour(int row, int col, Colour c) {
    tv->setColour(row,col,c);
    // gv->setColour(row,col,c);
};

void View:: setType(int row,int col,Type t) {
    tv->setType(row, col, t);
    // gv->setType(row, col, t);
};

void View:: setScore(int x) {
    tv->setScore(x);
    // gv->setScore(x);
};

void View:: setLevel(int x) {
    tv->setLevel(x);
    // gv->setLevel(x);
};

void View:: setMovesRemain(int x) {
    tv->setMovesRemain(x);
    // gv->setMovesRemain(x);
};

void View:: setHiScore(int x) {
    tv->setHiScore(x);
    // gv->setHiScore(x);
};

void View:: swap(int r,int c, Direction d) {
    tv->swap(r,c,d);
    // gv->swap(r,c,d);
};

void View:: drop(int col, Colour c, Type t) {
    tv->drop(col,c,t);
    // gv->drop(col,c,t);
};

void View:: fall(int row, int col, int distance) {
    tv->fall(row,col,distance);
    // gv->fall(row,col,distance);
};

void View:: destory(int r,int c) {
    tv->destory(r,c);
    // gv->destory(r,c);
};

void View:: end() {
    tv->end();
    // gv->end();
};

void View:: restart(int size) {
    tv->restart(size);
    // gv->restart(size);
};
