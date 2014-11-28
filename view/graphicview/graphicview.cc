#include "graphicview.h"
using namespace std;

/* MoveAnimation */
MoveAnimation:: MoveAnimation(GraphicCell* gc) {
    this->target = gc;
};

bool MoveAnimation:: during() {
    return (this->target->x != this->desX || this->target->y != this->desY);
};

void MoveAnimation:: animate() {
    // cerr << "animate: ";
    cerr << this->target->x << " " << this->desX << endl;
    cerr << this->target->y << " " << this->desY << endl;
    if (this->target->x < this->desX) {this->target->x++;} 
    else if (this->target->x > this->desX) {this->target->x--;}
    if (this->target->y < this->desY) {this->target->y++;} 
    else if (this->target->y > this->desY) {this->target->y--;}
}

void MoveAnimation:: to(int desX, int desY) {
    this->desX = desX;
    this->desY = desY;
    this->start();
}

/* GraphicCell */

GraphicCell:: GraphicCell() {
    this->move = new MoveAnimation(this);
}

void GraphicCell:: draw() const {
    this->window->fillRectangle(this->y, this->x, outer->cellSize, outer->cellSize, static_cast<int>(this->colour));
}


/* GraphicView */

GraphicView:: GraphicView(int size) {
    #if DEBUG_GRAPHIC
        fprintf(stderr,"NEW GRAPHICVIEW\n");
    #endif
    this->windowWidth = 500;  // default
    this->windowHeight = 500; // default
    this->marginLeft = 0;     // default
    this->marginTop = 0;      // default
    this->cellSize = 50;      // default
    this->fps = 1000/15;      // default
    this->window = new Xwindow(this->windowWidth, this->windowHeight);
    this->init(size);
}

GraphicView:: ~GraphicView() {
    #if DEBUG_GRAPHIC
        fprintf(stderr,"GRAPHICVIEW DESTROIED\n");
    #endif
    this->end();
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
            this->board[i][j] = new GraphicCell;
            // init
            this->board[i][j]->x = this->marginTop + i * this->cellSize;
            this->board[i][j]->y = this->marginLeft + j * this->cellSize;
            this->board[i][j]->window = this->window;
            this->board[i][j]->outer = this;
        }
    }
    #if DEBUG_GRAPHIC
        fprintf(stderr,"GraphicView:: init done");
    #endif
    
    this->main = new thread(&GraphicView::refresh, this);
}

void GraphicView:: end() {
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            delete this->board[i][j];
        }
        delete [] this->board[i];
    }
    delete [] this->board;
    if (this->main->joinable()) {this->main->join();}
    delete this->main;
}

void GraphicView:: draw() const {
    // for (int i = 0; i < this->size; i++) {
    //     for (int j = 0; j < this->size; j++) {
    //         this->board[i][j]->draw();
    //     }
    // }    
};  


void GraphicView:: setColour(int r, int c, Colour x) {
    this->board[r][c]->colour = x; 
};

void GraphicView:: setType(int,int,Type) {
    
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

void GraphicView:: setLocked(int,int,bool) {};

void GraphicView:: swap(int r, int c, Direction d) {
    GraphicCell** gc1 = &this->board[r][c];
    GraphicCell** gc2 ;
    int newX = (*gc1)->x;
    int newY = (*gc1)->y;
    int s = this->cellSize;
    switch (d) {
        case Up : { 
            gc2 = &this->board[r-1][c];
            newX = (*gc1)->x - s;
            break; 
        }
        case Down : {
            gc2 = &this->board[r+1][c];
            newX = (*gc1)->x + s;
            break;
        }
        case Left : {
            gc2 = &this->board[r][c-1];
            newY = (*gc1)->y - s;
            break;
        }
        case Right : {
            gc2 = &this->board[r][c+1];
            newY = (*gc1)->y + s;
            break;
        }
    }
    cerr << newX << newY;
    #if DEBUG_GRAPHIC
        fprintf(stderr, "this %d %d addr: %p", r,c, *gc1);
        fprintf(stderr,"Graphic swap %d %d from %d %d to %d %d, neigbhour from %d %d to %d %d",r,c,(*gc1)->x, (*gc1)->y, (*gc2)->x, (*gc2)->y, newX,newY, r*s, c*s);
    #endif
    (*gc1)->move->to(newX, newY);
    // (*gc2)->move->to(r*s, c*s);
    GraphicCell* tmp;
    tmp = *gc1;
    *gc1 = *gc2;
    *gc2 = tmp;
};

void GraphicView:: drop(int,Colour,Type) {
    
};

void GraphicView:: fall(int,int) {
    
};

void GraphicView:: destroy(int,int) {
    
};

void GraphicView:: restart(int) {
    
};

void GraphicView:: refresh() {
    // cerr << "refresh" << endl;
    this->window->fillRectangle(0,0,this->windowWidth,this->windowHeight, White);
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            this->board[i][j]->draw();
        }
    }    
    this_thread:: sleep_for(chrono::milliseconds(this->fps));
    this->refresh();
}
