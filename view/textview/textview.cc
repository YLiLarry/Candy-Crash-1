#include "textview.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

bool CellTextView:: empty() {
    return (this->colour == EMPTY);
}

void CellTextView:: draw() {
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
    #if DEBUG
        cerr << ">> TEXTVIEW CONSTRUCTOR START" << endl;
    #endif
    
    this->size = size;
    this->board = new CellTextView*[size];
    
    for (int i = 0; i < this->size; i++) {
        
        #if DEBUG_VIEW
            cerr << ">> TEXTVIEW CONSTRUCTOR i = " << i << endl;
        #endif
        
        this->board[i] = new CellTextView[size];
    } 
    
    #if DEBUG_VIEW
        cerr << ">> TEXTVIEW CONSTRUCTOR DONE" << endl;
    #endif
};

/* Destructor */
TextView:: ~TextView() {
    for (int i = 0; i < this->size; i++) {
            delete this->board[i];
    } 
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

void TextView:: setColour(int row, int col, COLOUR c) {this->board[row][col].colour = c; this->draw();}
void TextView:: setType(int row, int col, CELLTYPE t) {this->board[row][col].cellType = t; this->draw();}
void TextView:: set(vector<Cell*>) {
    // requires Cell* header
}
void TextView:: setScore(int x) {this->score = x; this->draw();}
void TextView:: setLevel(int x) {this->level = x; this->draw();}
void TextView:: setMovesRemain(int x) {this->movesRemain = x; this->draw();}
void TextView:: setHiScore(int x) {this->hiScore = x; this->draw();}


void _swap(CellTextView a, CellTextView b) {
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
            if (r <= 0) {return;}
            _swap(this->board[r][c], this->board[r-1][c]);
            break;
        }
        case DOWN : {
            if (r >= this->size - 1) {return;}
            _swap(this->board[r][c], this->board[r+1][c]);
            break;
        }
        case LEFT : {
            if (c <= 0) {return;}
            _swap(this->board[r][c], this->board[r][c-1]);
            break;
        }
        case RIGHT : {
            if (c >= this->size - 1) {return;}
            _swap(this->board[r][c], this->board[r][c+1]);
            break;
        }
    }
    this->draw();
}

void TextView:: destory(int r, int c) {
    this->draw();
}
void TextView:: destory(vector<Cell*>) {
    // requires Cell* header
}
