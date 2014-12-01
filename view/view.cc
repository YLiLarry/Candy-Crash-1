#include "view.h"
#include <unistd.h>
using namespace std;

View:: View(int size) {
    tv = new TextView(size);
    
    if (! Global:: TEXTMODE) {
        gv = new GraphicView(size);
    }
}

View:: ~View() {
    delete tv;
    
    if (! Global:: TEXTMODE) {
        delete gv;
    }
}

void View:: init(int size) {    
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"init %d\"\n", size);
    #endif
    tv->init(size);
    
    if (! Global:: TEXTMODE) {
        gv->init(size);
    }
};

void View:: draw() {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"draw\"\n");
    #endif
    tv->draw();
    
    if (! Global:: TEXTMODE) {
        gv->draw();
    }
};

void View:: setColour(int row, int col, Colour c) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setColour %d %d %c\"\n", row, col, colour2char(c));
    #endif
    tv->setColour(row,col,c);
    
    if (! Global:: TEXTMODE) {
        gv->setColour(row,col,c);
    }
};

void View:: setType(int row,int col,Type t) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setType %d %d %c\"\n", row,col, type2char(t));
    #endif
    tv->setType(row, col, t);
    
    if (! Global:: TEXTMODE) {
        gv->setType(row, col, t);
    }
};

void View:: setScore(int x) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setScore %d\"\n", x);
    #endif
    tv->setScore(x);
    
    if (! Global:: TEXTMODE) {
        gv->setScore(x);
    }
};

void View:: setLevel(int x) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setLevel %d\"\n", x);
    #endif
    tv->setLevel(x);
    
    if (! Global:: TEXTMODE) {
        gv->setLevel(x);
    }
};

void View:: setLocked(int r, int c, bool x) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setLocked %d %d %d\"\n", r,c,x);
    #endif
    tv->setLocked(r,c,x);
    
    if (! Global:: TEXTMODE) {
        gv->setLocked(r,c,x);
    }
};

void View:: setMovesRemain(int x) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setMovesRemain %d\"\n", x);
    #endif
    tv->setMovesRemain(x);
    
    if (! Global:: TEXTMODE) {
        gv->setMovesRemain(x);
    }
};

void View:: setHiScore(int x) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setHiScore %d\"\n", x);
    #endif
    tv->setHiScore(x);
    
    if (! Global:: TEXTMODE) {
        gv->setHiScore(x);
    }
};

void View:: swap(int r,int c, Direction d) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"swap %d %d %s\"\n", r,c,dir2str(d).c_str());
    #endif
    tv->swap(r,c,d);
    
    if (! Global:: TEXTMODE) {
        gv->swap(r,c,d);
    }
};

void View:: drop(int col, Colour c, Type t) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"drop %d %c %c\"\n", col,colour2char(c),type2char(t));
    #endif
    tv->drop(col,c,t);
    
    if (! Global:: TEXTMODE) {
        gv->drop(col,c,t);
    }
};

void View:: fall(int row, int col) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"fall %d %d\"\n", row,col);
    #endif
    tv->fall(row,col);
    
    if (! Global:: TEXTMODE) {
        gv->fall(row,col);
    }
};

void View:: destroy(int r,int c) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"destroy %d %d\"\n", r,c);
    #endif
    tv->destroy(r,c);
    
    if (! Global:: TEXTMODE) {
        gv->destroy(r,c);
    }
};

void View:: end() {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"end\"\n");
    #endif
    tv->end();
    
    if (! Global:: TEXTMODE) {
        gv->end();
    }
};

void View:: restart(int size) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"restart %d\"\n",size);
    #endif
    tv->restart(size);
    
    if (! Global:: TEXTMODE) {
        gv->restart(size);
    }
};

void View:: print(const string& str) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"print %s\"\n", str.c_str());
    #endif
    tv->print(str);
    
    if (! Global:: TEXTMODE) {
        gv->print(str);
    }
}

void View:: setLabel(const string& str) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setLabel %s\"\n", str.c_str());
    #endif
    tv->setLabel(str);
    
    if (! Global:: TEXTMODE) {
        gv->setLabel(str);
    }
}
