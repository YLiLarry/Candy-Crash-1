#include "graphicview.h"
#include <algorithm>
using namespace std;

/* MoveAnimation */

bool MoveAnimation:: during(std::vector<int> c) {
    return (this->target->x != c[0] || this->target->y != c[1]);
};

void MoveAnimation:: animate(std::vector<int> c) {
    if (this->target->x < c[0]) {this->target->x += 1;} 
    else if (this->target->x > c[0]) {this->target->x -= 1;}
    if (this->target->y < c[1]) {this->target->y += 1;} 
    else if (this->target->y > c[1]) {this->target->y -= 1;}
    this->target->needDraw = true;
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

/* FallAnimation */
bool FallAnimation:: during(std::vector<int> v) {
    if (target->x < v[0]) {return true;}
    target->outer->fallingNum[target->col]--;
    target->x = v[0];
    target->needDraw = true;
    if (target->row+1 < target->outer->size) {target->outer->board[target->row+1][target->col]->needDraw = true;}
    if (target->row > 0) {target->outer->board[target->row-1][target->col]->needDraw = true;}
    return false;
}

void FallAnimation:: animate(std::vector<int> v) {
    int midX = v[1];
    if (target->x < midX) {
        target->speed+=0.2;
    } else if (target->speed > 1) {
        target->speed-=0.2;
    } else {
        target->speed = 1;
    }
    target->x += target->speed;
    target->needDraw = true;
    if (target->row+1 < target->outer->size) {target->outer->board[target->row+1][target->col]->needDraw = true;}
    if (target->row > 0) {target->outer->board[target->row-1][target->col]->needDraw = true;}
}

void FallAnimation:: to(int d, int m) {
    vector<int> v;
    v.push_back(d);
    v.push_back(m);
    this->push(v);
}

/* GraphicCell */

GraphicCell:: GraphicCell(int r, int c, GraphicView* outer) {
    this->outer = outer;
    this->lx = this->x = r * outer->cellSize + outer->marginTop;
    this->ly = this->y = c * outer->cellSize + outer->marginLeft;
    this->row = r;
    this->col = c;
    this->speed = 1;
    this->needDraw = true;
    this->colour = Empty;
    this->window = outer->window;
    this->move = MoveAnimation(this, outer->fps);
    this->fall = FallAnimation(this, outer->fps);
}

void GraphicCell:: draw() {
    if (Global:: OPTIMIZED && ! this->needDraw) {return;}
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
    this->window->drawString(this->y + 5, this->x + outer->cellSize / 2 + 10, str, 4);    
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
    this->fallingNum = vector<int>(size,0);
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
    #if DEBUG_GRAPHIC
        fprintf(stderr,"CALL GRAPHICVIEW DESTROY\n");
    #endif
    this->end();
    delete this->window;
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
            this->board[i][j] = new GraphicCell(i, j, this);
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
    
    #if DEBUG_GRAPHIC
        fprintf(stderr,"delete board");
    #endif
    
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
    this->waitAllAnimationsFinish();
    this_thread:: sleep_for(chrono::seconds(1));
};

void GraphicView:: setMovesRemain(int) {
    
};

void GraphicView:: setHiScore(int) {
    
};


void GraphicView:: print(const std::string& s) {
};

void GraphicView:: setLabel(const std::string& s) {
};

void GraphicView:: setLocked(int r,int c,bool b) {this->board[r][c]->lock = b; this->board[r][c]->needDraw = true;}

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
    (*gc1)->move.to(newX, newY);
    (*gc2)->move.to(r*s, c*s);
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
    GraphicCell*& nc = this->board[0][column];
    nc->x = 0 - (this->cellSize * (this->fallingNum[column]+1));

    nc->y = this->marginLeft + column * this->cellSize;
    // this_thread::sleep_for(chrono::milliseconds(250)); // to be deleted
    nc->colour = colour;
    nc->cellType = type;
    this->fall(0, column);
}

void GraphicView:: fall(int r, int c) {
    #if DEBUG_GRAPHIC
        fprintf(stderr,">> square %d %d fall ", r, c);
    #endif
    this->fallingNum[c]++;
    GraphicCell*& ori = this->board[r][c];
    int i = r;
    while (i < this->size - 1 && this->board[i+1][c]->colour == Empty) {i++;}
    #if DEBUG_GRAPHIC
        fprintf(stderr,"to row %d\n", i);
    #endif
    GraphicCell*& des = this->board[i][c];
    if (i > 0) {
        des->x = r * this->cellSize;
        des->y = c * this->cellSize;
        std::swap(des->row, ori->row);
        std::swap(des->col, ori->col);
        des->needDraw = true;
    }
    ori->speed = 1;
    ori->fall.to(i*this->cellSize, (i*this->cellSize + ori->x) / 2);
    // ori->move.to(i*this->cellSize, c*this->cellSize);
    //this_thread::sleep_for(chrono::milliseconds(250)); // to be deleted
    std::swap(ori, des);
};

void GraphicView:: destroy(int r,int c) {
    #if DEBUG_GRAPHIC
        fprintf(stderr,">> destory %d %d", r, c);
    #endif
        this->waitAllAnimationsFinish();
        this->fallingNum = vector<int>(this->size, 0);
        this->board[r][c]->colour = Empty;
        this->board[r][c]->cellType = Basic;
        this->board[r][c]->needDraw = true;
};

void GraphicView:: destroy(vector<int> rows, vector<int> cols) {
    int limR = rows.size();
    int limC = cols.size();
    if (limC != limR) {throw string("ERROR: The vectors' sizes passed in View::destroy don't match: rows.size() = ") + to_string(limR) + ", cols.size() = " + to_string(limC);}
    this->waitAllAnimationsFinish();
    this->fallingNum = vector<int>(this->size, 0);
    for (int i = 0; i < limR; i++) {
        this->destroy(rows[i], cols[i]);
    }
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
    return this->refresh();
}

void GraphicView:: waitAllAnimationsFinish() {
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            this->board[i][j]->move.join();
            this->board[i][j]->fall.join();
        }
    }
}
