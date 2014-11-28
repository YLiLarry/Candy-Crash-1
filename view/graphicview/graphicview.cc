#include "graphicview.h"
using namespace std;

void GraphicView:: refresh() {
    // cerr << "refresh" << endl;
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            this->board[i][j].draw();
        }
    }
    this_thread:: sleep_for(chrono::milliseconds(50));
    this->refresh();
}

void GraphicView::GraphicCell:: draw() const {
    this->window->fillRectangle(this->y, this->x, outer->cellSize, outer->cellSize, static_cast<int>(this->colour));
}

GraphicView:: GraphicView(int size) {
    #if DEBUG_GRAPHIC
        fprintf(stderr,"NEW GRAPHICVIEW\n");
    #endif
    this->windowWidth = 500;  // default
    this->windowHeight = 500; // default
    this->marginLeft = 0;     // default
    this->marginTop = 0;      // default
    this->cellSize = 50;      // default
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
    this->board = new GraphicCell*[size];
    for (int i = 0; i < size; i++) {
        this->board[i] = new GraphicCell[size];
        for (int j = 0; j < size; j++) {
            // init
            this->board[i][j].x = this->marginTop + i * this->cellSize;
            this->board[i][j].y = this->marginLeft + j * this->cellSize;
            this->board[i][j].window = this->window;
            this->board[i][j].outer = this;
        }
    }
    #if DEBUG_GRAPHIC
        fprintf(stderr,"GraphicView:: init done");
    #endif
    
    this->main = new thread(&GraphicView::refresh, this);
    this->main->detach();
}

void GraphicView:: end() {
    for (int i = 0; i < this->size; i++) {
        delete [] this->board[i];
    }
    delete [] this->board;
    if (this->main->joinable()) {this->main->join();}
    delete this->main;
}

void GraphicView:: draw() const {};  


void GraphicView:: setColour(int r, int c, Colour x) {
    this->board[r][c].colour = x; 
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

void GraphicView:: swap(int,int,Direction) {
    
};

void GraphicView:: drop(int,Colour,Type) {
    
};

void GraphicView:: fall(int,int) {
    
};

void GraphicView:: destory(int,int) {
    
};

void GraphicView:: restart(int) {
    
};

