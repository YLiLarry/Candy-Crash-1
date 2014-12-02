#include "view.h"
#include <unistd.h>
using namespace std;

/*  Constructor:  View
    Description:  Construct an empty View object without initializing the grid. A text view and a graphic view will be opened
    Parameters:   size
*/

View:: View(int size) {
    tv = new TextView(size);
    
    if (! Global:: TEXTMODE) {
        gv = new GraphicView(size);
    }
}

/* Destructor:   View
   Description:  When called, destroy both text and graphic view. The window of graphic view will be destroied as well.
*/
View:: ~View() {
    delete tv;
    
    if (! Global:: TEXTMODE) {
        delete gv;
    }
}

/*  Method:       View
    Description:  initialize all squares on both view's grid to Empty colour and Basic type
    Parameters:   size
    Return:       void
*/
void View:: init(int size) {    
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"init %d\"\n", size);
    #endif
    tv->init(size);
    
    if (! Global:: TEXTMODE) {
        gv->init(size);
    }
};

/*  Method:       draw
    Description:  redraw the text and graphic grid
    Parameters:   None
    Return:       void
*/
void View:: draw() {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"draw\"\n");
    #endif
    tv->draw();
    
    if (! Global:: TEXTMODE) {
        gv->draw();
    }
};

/*  Method:       setColour
    Description:  set the square at row, col to colour c
    Parameters:   int row
                  int col
                  Colour c 
    Return:       void
*/
void View:: setColour(int row, int col, Colour c) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setColour %d %d %c\"\n", row, col, colour2char(c));
    #endif
        
    tv->setColour(row,col,c);
    
    if (! Global:: TEXTMODE) {
        gv->setColour(row,col,c);
    }
};

/*  Method:       setType
    Description:  set the square at row, col to type c
    Parameters:   int row
                  int col
                  Type c 
    Return:       void
*/
void View:: setType(int row,int col,Type t) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setType %d %d %c\"\n", row,col, type2char(t));
    #endif
    tv->setType(row, col, t);
    
    if (! Global:: TEXTMODE) {
        gv->setType(row, col, t);
    }
};

/*  Method:       setScore
    Description:  set the score displayed on textview to x (graphic view has no implementation yet)
    Parameters:   int x
    Return:       void
*/
void View:: setScore(int x) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setScore %d\"\n", x);
    #endif
    tv->setScore(x);
    
    if (! Global:: TEXTMODE) {
        gv->setScore(x);
    }
};

/*  Method:       setLevel
    Description:  set the level displayed on textview to x (graphic view has no implementation yet)
    Parameters:   int x
    Return:       void
*/
void View:: setLevel(int x) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setLevel %d\"\n", x);
    #endif
    tv->setLevel(x);
    
    if (! Global:: TEXTMODE) {
        gv->setLevel(x);
    }
};

/*  Method:       setLocked
    Description:  set the lock status of a sqaure at row column on both view
    Parameters:   int row
                  int column
                  bool locked?
    Return:       void
*/
void View:: setLocked(int r, int c, bool x) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setLocked %d %d %d\"\n", r,c,x);
    #endif
    tv->setLocked(r,c,x);
    
    if (! Global:: TEXTMODE) {
        gv->setLocked(r,c,x);
    }
};

/*  Method:       setMovesRemain
    Description:  set the movesRemain displayed on textview to x (graphic view has no implementation yet)
    Parameters:   int x
    Return:       void
*/
void View:: setMovesRemain(int x) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setMovesRemain %d\"\n", x);
    #endif
    tv->setMovesRemain(x);
    
    if (! Global:: TEXTMODE) {
        gv->setMovesRemain(x);
    }
};

/*  Method:       setHiScore
    Description:  set the HiScore displayed on textview to x (graphic view has no implementation yet)
    Parameters:   int x
    Return:       void
*/
void View:: setHiScore(int x) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setHiScore %d\"\n", x);
    #endif
    tv->setHiScore(x);
    
    if (! Global:: TEXTMODE) {
        gv->setHiScore(x);
    }
};

/*  Method:       swap
    Description:  swap a square at row, column with it's neighbour in direction d (defined in /public/global.h)
    Parameters:   int row
                  int column
                  Direction d (defined in /public/global.h)
    Return:       void
*/
void View:: swap(int r,int c, Direction d) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"swap %d %d %s\"\n", r,c,dir2str(d).c_str());
    #endif
    tv->swap(r,c,d);
    
    if (! Global:: TEXTMODE) {
        gv->swap(r,c,d);
    }
};

/*  Method:       drop
    Description:  drop a new square of colour c type t at column col and play the animation
    Parameters:   int col
                  Colour c
                  Type t
    Return:       void
*/
void View:: drop(int col, Colour c, Type t) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"drop %d %c %c\"\n", col,colour2char(c),type2char(t));
    #endif
                
    tv->drop(col,c,t);
    
    if (! Global:: TEXTMODE) {
        gv->drop(col,c,t);
    }
};

/*  Method:       fall
    Description:  let the square at r c to fall down until meeting the first non-empty square, animation is played
    Parameters:   int r
                  int c
    Return:       void
*/
void View:: fall(int row, int col) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"fall %d %d\"\n", row,col);
    #endif
    tv->fall(row,col);
    
    if (! Global:: TEXTMODE) {
        gv->fall(row,col);
    }
};

/*  Method:       destroy
    Description:  make a square r c 'explode'. play the animation.
    Parameters:   int r
                  int c
    Return:       void
*/
void View:: destroy(int r,int c) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"destroy %d %d\"\n", r,c);
    #endif
    tv->destroy(r,c);
    
    if (! Global:: TEXTMODE) {
        gv->destroy(r,c);
    }
};

void View:: destroy(std::vector<int> r, std::vector<int> c) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"destroy %d %d\"\n", r,c);
    #endif
    tv->destroy(r,c);
    
    if (! Global:: TEXTMODE) {
        gv->destroy(r,c);
    }
};

/*  Method:       end
    Description:  Terminate both graphic and text view 
    Parameters:   None
    Return:       void
*/
void View:: end() {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"end\"\n");
    #endif
    tv->end();
    
    if (! Global:: TEXTMODE) {
        gv->end();
    }
};

/*  Method:       restart
    Description:  this method calls View::end() and then View::init(size);
    Parameters:   int size
    Return:       void
*/
void View:: restart(int size) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"restart %d\"\n",size);
    #endif
    tv->restart(size);
    
    if (! Global:: TEXTMODE) {
        gv->restart(size);
    }
};

/*  Method:       print
    Description:  print a string to stdout
    Parameters:   const string& str
    Return:       void
*/
void View:: print(const string& str) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"print %s\"\n", str.c_str());
    #endif
    tv->print(str);
    
    if (! Global:: TEXTMODE) {
        gv->print(str);
    }
}

/*  Method:       setLabel
    Description:  print a string to the place below the 'current level:' and 'current score' in the text view, and keep the new string there when redraw.
    Parameters:   const string& str
    Return:       void
*/
void View:: setLabel(const string& str) {
    #if DEBUG_VIEW
        fprintf(stderr,"VIEW >> call method \"setLabel %s\"\n", str.c_str());
    #endif
    tv->setLabel(str);
    
    if (! Global:: TEXTMODE) {
        gv->setLabel(str);
    }
}
