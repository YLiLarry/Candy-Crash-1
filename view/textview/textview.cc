#include "textview.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

void TextCell:: draw() const {
    string str = "_";
    switch (this->cellType) {
        case Basic : str += "_"; break;
        case Lateral : str += "h"; break;
        case Upright : str += "v"; break;
        case Unstable : str += "b"; break;
        case Psychedelic : str += "p"; break;
    }
    switch (this->colour) {
        case Empty : str += "_"; break;
        case White : str += "0"; break;
        case Red : str += "1"; break;
        case Green : str += "2"; break;
        case Blue : str += "3"; break;
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
    #if DEBUG
        cout << endl;
        for (int i = 0; i < this->size; i++) {
            for (int j = 0; j < this->size; j++) {
                this->board[i][j].draw();
                cout << " ";
            }
            cout << endl;
        } 
    #else
        cout << endl;
        cout << "Level:\t" << this->level << endl;
        cout << "Score:\t" << this->score << endl;
        cout << "----------" << endl << endl;
        for (int i = 0; i < this->size; i++) {
            for (int j = 0; j < this->size; j++) {
                this->board[i][j].draw();
                cout << " ";
            }
            cout << endl;
        }         
        cout << endl;
    #endif
}

void TextView:: setColour(int row, int col, Colour c) {this->board[row][col].colour = c;}
void TextView:: setType(int row, int col, Type t) {this->board[row][col].cellType = t;}
// void TextView:: set(vector<Cell*>) {
//     // requires Cell* header
// }
void TextView:: setScore(int x) {this->score = x;}
void TextView:: setLevel(int x) {this->level = x;}
void TextView:: setMovesRemain(int x) {this->movesRemain = x;}
void TextView:: setHiScore(int x) {this->hiScore = x;}


void _swap(TextCell& a, TextCell& b) {
    Colour c;
    c = a.colour;
    a.colour = b.colour;
    b.colour = c;
    Type t;
    t = a.cellType;
    a.cellType = b.cellType;
    b.cellType = t;
}

void TextView:: swap(int r, int c, Direction d) {
    switch (d) {
        case Up : {
            // if (r <= 0) {return;}
            _swap(this->board[r][c], this->board[r-1][c]);
            break;
        }
        case Down : {
            // if (r >= this->size - 1) {return;}
            _swap(this->board[r][c], this->board[r+1][c]);
            break;
        }
        case Left : {
            // if (c <= 0) {return;}
            _swap(this->board[r][c], this->board[r][c-1]);
            break;
        }
        case Right : {
            // if (c >= this->size - 1) {return;}
            _swap(this->board[r][c], this->board[r][c+1]);
            break;
        }
    }
}

void TextView:: drop(int column, Colour colour, Type type = Basic) {
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
    #if DEBUG_VIEW
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

void TextView:: print(const string& str) {
    cout << str << endl;
}
