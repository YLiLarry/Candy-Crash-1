#include "textview.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

void TextCell:: draw() const {
    string str = "_";
    switch (this->cellType) {
        case BASIC : str += "_"; break;
        case LATERAL : str += "h"; break;
        case UPRIGHT : str += "v"; break;
        case UNSTABLE : str += "b"; break;
        case PSYCHEDELIC : str += "p"; break;
    }
    switch (this->colour) {
        case EMPTY : str += "_"; break;
        case WHITE : str += "0"; break;
        case RED : str += "1"; break;
        case GREEN : str += "2"; break;
        case BLUE : str += "3"; break;
    }
    cout << str;
}

/*  Constructor:  TextView
    Parameters:   int size - the size of the board to initialize.
    Description:  This constructor initilizes all cells to the white colour, basic squares.
*/
TextView:: TextView(int size) {
    this->init(size);
};

/* Destructor */
TextView:: ~TextView() {
    this->end();
}

void TextView:: draw() const {
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            this->board[i][j].draw();
            cout << " ";
        }
        cout << endl;
    } 
}

void TextView:: setColour(int row, int col, COLOUR c) {this->board[row][col].colour = c;}
void TextView:: setType(int row, int col, CELLTYPE t) {this->board[row][col].cellType = t;}
// void TextView:: set(vector<Cell*>) {
//     // requires Cell* header
// }
void TextView:: setScore(int x) {this->score = x;}
void TextView:: setLevel(int x) {this->level = x;}
void TextView:: setMovesRemain(int x) {this->movesRemain = x;}
void TextView:: setHiScore(int x) {this->hiScore = x;}


void _swap(TextCell a, TextCell b) {
    COLOUR c;
    c = a.colour;
    a.colour = b.colour;
    b.colour = c;
    CELLTYPE t;
    t = a.cellType;
    a.cellType = b.cellType;
    b.cellType = t;
}

void TextView:: swap(int r, int c, DIRECTION d) {
    switch (d) {
        case UP : {
            // if (r <= 0) {return;}
            _swap(this->board[r][c], this->board[r-1][c]);
            break;
        }
        case DOWN : {
            // if (r >= this->size - 1) {return;}
            _swap(this->board[r][c], this->board[r+1][c]);
            break;
        }
        case LEFT : {
            // if (c <= 0) {return;}
            _swap(this->board[r][c], this->board[r][c-1]);
            break;
        }
        case RIGHT : {
            // if (c >= this->size - 1) {return;}
            _swap(this->board[r][c], this->board[r][c+1]);
            break;
        }
    }
}

void TextView:: drop(int column, COLOUR colour, CELLTYPE type = BASIC) {
    int row = 0;
    while (row < this->size && this->board[row][column].empty()) {row++;}
    this->board[row][column].colour = colour;
    this->board[row][column].cellType = type;
}

void TextView:: fall(int r, int c, int d) {
    _swap(this->board[r][c], this->board[r+d][c]);
};


void TextView:: destory(int r, int c) {
    this->board[r][c].setEmpty();
}

void TextView:: init(int size) {
    #if DEBUG
        cerr << ">> TEXTVIEW CONSTRUCTOR START" << endl;
    #endif
    
    this->size = size;
    this->board = new TextCell*[size];
    
    for (int i = 0; i < this->size; i++) {
        
        #if DEBUG_VIEW
            cerr << ">> TEXTVIEW CONSTRUCTOR i = " << i << endl;
        #endif
        
        this->board[i] = new TextCell[size];
    } 
    
    #if DEBUG_VIEW
        cerr << ">> TEXTVIEW CONSTRUCTOR DONE" << endl;
    #endif
}

void TextView:: end() {
    #if DEBUG_VIEW
        cerr << ">> TEXTVIEW DESTORIED" << endl;
    #endif
        
    for (int i = 0; i < this->size; i++) {
            delete [] this->board[i];
    }
    
    delete [] this->board;
}

void TextView:: restart(int size = 0) {
    size ? this->init(size) : this->init(this->size);
    this->end();
}
