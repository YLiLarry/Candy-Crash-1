#include "graphicview.h"
#include <algorithm>
using namespace std;

/* MoveAnimation */
MoveAnimation:: MoveAnimation(GraphicCell* gc) {
    #if DEBUG_GRAPHIC
        fprintf(stderr,"NEW MoveAnimation\n");
    #endif
    this->target = gc;
};

bool MoveAnimation:: during(std::vector<int> c) {
    // #if DEBUG_GRAPHIC
    //     if (this->target->x == c[0] && this->target->y == c[1]) {
    //         fprintf(stderr,"animate done:\ncurrent %p pos: %d %d, colour %d\n",
    //                 this->target, 
    //                 this->target->x, 
    //                 this->target->y,
    //                 this->target->colour);
    //     }
        
    // fprintf(stderr,"current %p pos: %d %d, colour %d\n",
    // this->target, 
    // this->target->x, 
    // this->target->y,
    // this->target->colour);
        
    // #endif
    return ((this->target->x != c[0] || this->target->y != c[1]) && (this->target->acc = 1));
};

void MoveAnimation:: animate(std::vector<int> c) {
    this->target->needDraw = true;
    if (this->target->x < c[0]) {this->target->x += 1;} 
    else if (this->target->x > c[0]) {this->target->x -= 1;}
    if (this->target->y < c[1]) {this->target->y += 1;} 
    else if (this->target->y > c[1]) {this->target->y -= 1;}
}

void MoveAnimation:: to(int desX, int desY) {
    #if DEBUG_GRAPHIC
        fprintf(stderr,"MoveAnimation from %d %d to %d %d\n", 
                this->target->x, this->target->y,
                desX, desY);
    #endif
    std::vector<int> c;
    c.push_back(desX);
    c.push_back(desY);
    this->push(c);
}

/* GraphicCell */

GraphicCell:: GraphicCell() {
    this->move = new MoveAnimation(this);
    this->x = 0;
    this->y = 0;
    this->lx = 0;
    this->ly = 0;
    this->acc = 1;
    this->needDraw = true;
    this->colour = Empty;
    this->window = NULL;
    this->outer = NULL;
}

void GraphicCell:: draw() {
    if (! this->needDraw) {return;}
    // x/y in the two calls might not be the same because of concurrency 
    int tmpX = this->x;
    int tmpY = this->y;
    this->window->fillRectangle(this->ly, this->lx, outer->cellSize, outer->cellSize, 4);
    this->window->fillRectangle(tmpY, tmpX, outer->cellSize, outer->cellSize, static_cast<int>(this->colour));
    this->lx = tmpX;
    this->ly = tmpY;
    // temp
    string str(this->lock ? "L" : " ");
    switch (this->cellType) {
        case Basic : str += " "; break;
        case Lateral : str += "h"; break;
        case Upright : str += "v"; break;
        case Unstable : str += "b"; break;
        case Psychedelic : str += "p"; break;
    }
    this->window->drawString(this->y + 5, this->x + outer->cellSize / 2 + 10, str, Xwindow::White);    
    this->needDraw = false;
}


/* GraphicView */

GraphicView:: GraphicView(int size) {
    #if DEBUG_GRAPHIC
        fprintf(stderr,"NEW GRAPHICVIEW\n");
    #endif
    this->toggle = true;
    this->board = NULL;
    this->window = NULL;
    this->droppingNum = vector<int>(size,0);
    #if DEBUG_GRAPHIC
        for (int i = 0; i < this->droppingNum.size(); i++) {
            fprintf(stderr,"this->droppingNum[%d] = %d\n", i, this->droppingNum[i]);
        }
    #endif
    
    this->windowWidth = 500;  // default
    this->windowHeight = 500; // default
    this->marginLeft = 0;     // default
    this->marginTop = 0;      // default
    this->cellSize = 50;      // default
    this->fps = std::chrono::milliseconds(1000/50);      // default
    this->window = new Xwindow(this->windowWidth, this->windowHeight);
    this->init(size);
}

GraphicView:: ~GraphicView() {
    this->end();
    #if DEBUG_GRAPHIC
        fprintf(stderr,"GRAPHICVIEW DESTROIED\n");
    #endif
}

void GraphicView:: init(int size) {
    #if DEBUG_GRAPHIC
        fprintf(stderr,"GraphicView:: init(%d)\n", size);
    #endif
    this->size = size;
    this->board = new GraphicCell**[size];
    for (int i = 0; i < size; i++) {
        this->board[i] = new GraphicCell*[size];
        for (int j = 0; j < size; j++) {
            GraphicCell*& gc = this->board[i][j] = new GraphicCell;
            // init
            gc->x = gc->lx = this->marginTop + i * this->cellSize;
            gc->y = gc->ly = this->marginLeft + j * this->cellSize;
            gc->window = this->window;
            gc->outer = this;
        }
    }
    // #if DEBUG_GRAPHIC
    //     fprintf(stderr,"GraphicView:: init done");
    // #endif
    
    this->td = new thread(&GraphicView::refresh, this);
}

void GraphicView:: end() {
    #if DEBUG_GRAPHIC
        fprintf(stderr,"call GraphicView:: end()\n");
    #endif

    #if DEBUG_GRAPHIC
        fprintf(stderr,"thread try join %p\n", this->td);
        if (! this->td->joinable()) {cerr << "cannot join" << endl;}
    #endif
    this->toggle = false;
    this->td->join();
    #if DEBUG_GRAPHIC
        fprintf(stderr,"refresh thread join\n");
    #endif
    delete this->td;
    delete this->window;
    
    // refresh was still using the board!
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            delete this->board[i][j];
        }
        delete [] this->board[i];
    }
    delete [] this->board;
}

void GraphicView:: draw() {
    // for (int i = 0; i < this->size; i++) {
    //     for (int j = 0; j < this->size; j++) {
    //         this->board[i][j]->draw();
    //     }
    // }    
};  


void GraphicView:: setColour(int r, int c, Colour x) {
    this->board[r][c]->colour = x; 
    this->board[r][c]->needDraw = true;
};

void GraphicView:: setType(int r,int c,Type t) {
    this->board[r][c]->cellType = t;
    this->board[r][c]->needDraw = true;
};

void GraphicView:: setScore(int) {
    
};

void GraphicView:: setLevel(int) {
    
};

void GraphicView:: setMovesRemain(int) {
    
};

void GraphicView:: setHiScore(int) {
    
};


void GraphicView:: print(const std::string& s) {
};

void GraphicView:: setLabel(const std::string& s) {
};

void GraphicView:: setLocked(int r,int c,bool b) {this->board[r][c]->lock = b;}

void GraphicView:: swap(int r, int c, Direction d) {
    GraphicCell** gc1 = &this->board[r][c];
    GraphicCell** gc2;
    int s = this->cellSize;
    int newX = r * s;
    int newY = c * s;
    switch (d) {
        case Up : { 
            gc2 = &this->board[r-1][c];
            newX = (r-1) * s;
            break; 
        }
        case Down : {
            gc2 = &this->board[r+1][c];
            newX = (r+1) * s;
            break;
        }
        case Left : {
            gc2 = &this->board[r][c-1];
            newY = (c-1) * s;
            break;
        }
        case Right : {
            gc2 = &this->board[r][c+1];
            newY = (c+1) * s;
            break;
        }
    }
    (*gc1)->move->to(newX, newY);
    (*gc2)->move->to(r*s, c*s);
    this_thread:: sleep_for(chrono::seconds(1));
    GraphicCell* tmp;
    tmp = *gc1;
    *gc1 = *gc2;
    *gc2 = tmp;
};

void GraphicView:: drop(int column, Colour colour, Type type = Basic) {
    #if DEBUG_GRAPHIC
        fprintf(stderr,"new square at col %d:\ncolour %d, type %d", column, colour, type);
    #endif
    this->droppingNum[column]++;
    GraphicCell*& nc = this->board[0][column];
    // init
    // int i = 0;
    // while (i < this->size - 1 && this->board[i+1][column]->colour == Empty) {i++;}
    nc->x = 0 - (this->cellSize * this->droppingNum[column]);
    #if DEBUG_GRAPHIC
        for (int i = 0; i < this->droppingNum.size(); i++) {
            fprintf(stderr,"this->droppingNum[%d] = %d\n", i, this->droppingNum[i]);
        }
    #endif
    cerr << "nc->x = " << nc->x;
    nc->y = this->marginLeft + column * this->cellSize;
    // this_thread::sleep_for(chrono::milliseconds(250)); // to be deleted
    // #if DEBUG_GRAPHIC
        // fprintf(stderr,"set colour to %d\nthe addrss is %p", colour, this->board[0][column]);
    // #endif
    nc->colour = colour;
    nc->cellType = type;
    this->fall(0, column);
    // nc->move->to(0, column*this->cellSize);
    // #if DEBUG_GRAPHIC
    //     fprintf(stderr,"drpp returns\n");
    // #endif
}

void GraphicView:: fall(int r, int c) {
    #if DEBUG_GRAPHIC
        fprintf(stderr,">> square %d %d fall ", r, c);
    #endif
    GraphicCell*& ori = this->board[r][c];
    int i = r;
    while (i < this->size - 1 && this->board[i+1][c]->colour == Empty) {i++;}
    #if DEBUG_GRAPHIC
        fprintf(stderr,"to row %d\n", i);
    #endif
    GraphicCell*& des = this->board[i][c];
    if (i > 0) {
        des->x = des->lx = r * this->cellSize;
        des->y = des->ly = c * this->cellSize;
    }
    // cerr << "checkpoint to" << ori->move << endl;
    ori->move->to(i*this->cellSize, c*this->cellSize);
    this_thread::sleep_for(chrono::milliseconds(250)); // to be deleted
    // #if DEBUG_GRAPHIC
    //     fprintf(stderr,"before swap ori = \"%p\", des = \"%p\"\n", this->board[r][c], this->board[i][c]);
    // #endif
    std::swap(ori, des);
    // #if DEBUG_GRAPHIC
    //     fprintf(stderr,"after swap!! ori = \"%p\", des = \"%p\"\n", this->board[r][c], this->board[i][c]);
    // #endif
    // #if DEBUG_GRAPHIC
    //     fprintf(stderr,">> fall returns\n");
    // #endif
};

void GraphicView:: destroy(int r,int c) {
    #if DEBUG_GRAPHIC
        fprintf(stderr,">> destory %d %d", r, c);
    #endif
    // if (this->board[r][c]->move->done()) {
        this->waitAllAnimationsFinish();
        this->droppingNum = vector<int>(this->size, 0);
        this->board[r][c]->colour = Empty;
        this->board[r][c]->needDraw = true;
    // } else {
        // this->destroy(r,c); // retry
    // }
};

void GraphicView:: restart(int) {
    
};

void GraphicView:: refresh() {
    if (! this->toggle) {
        #if DEBUG_GRAPHIC
            fprintf(stderr,"refresh thread end\n");
        #endif
        return;
    }
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            this->board[i][j]->draw();
        }
    }    
    this_thread:: sleep_for(this->fps);
    this->refresh();
}

void GraphicView:: waitAllAnimationsFinish() {
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            this->board[i][j]->move->join();
        }
    }
}
