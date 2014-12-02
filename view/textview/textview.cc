#include "textview.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

void TextCell:: draw() {
	
    string str(this->lock ? "l" : "_");
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
    #if DEBUG_VIEW
        cerr << ">> CALL TEXTVIEW DESTROY" << endl;
    #endif
    this->end();
    #if DEBUG_VIEW
        cerr << ">> TEXTVIEW DESTROIED" << endl;
    #endif    
}

void TextView:: draw() {
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
        if (this->customLabel.length()) {cout << this->customLabel << endl;}
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

void TextView:: setColour(int row, int col, Colour c) {
    if (c == Empty) {throw string("ERROR: Attempted to set square ") + to_string(row) + " " + to_string(col) + " to Empty, use view::destroy instead.";}
    this->board[row][col].colour = c;
}
void TextView:: setType(int row, int col, Type t) {this->board[row][col].cellType = t;}
// void TextView:: set(vector<Cell*>) {
//     // requires Cell* header
// }
void TextView:: setScore(int x) {this->score = x;}
void TextView:: setLevel(int x) {this->level = x;}
void TextView:: setLocked(int r, int c, bool x) {this->board[r][c].lock = x;}
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
    bool fail = false;
    switch (d) {
        case Up : {
            if (r <= 0) {fail = true;} 
            else {_swap(this->board[r][c], this->board[r-1][c]);}
            break;
        }
        case Down : {
            if (r >= this->size - 1) {fail = true;} 
            else {_swap(this->board[r][c], this->board[r+1][c]);}
            break;
        }
        case Left : {
            if (c <= 0) {fail = true;} 
            else {_swap(this->board[r][c], this->board[r][c-1]);}
            break;
        }
        case Right : {
            if (c >= this->size - 1) {fail = true;} 
            else {_swap(this->board[r][c], this->board[r][c+1]);}
            break;
        }
    }
    if (fail) {
        throw string("Error: Attempted to swap block ") + to_string(r) + " " + to_string(c) + " to " + dir2str(d);
    }
}

void TextView:: drop(int column, Colour colour, Type type = Basic) {
    if (colour == Empty) {throw string("Error: Attempted to drop an Empty block at column: ") + to_string(column);}
    this->board[0][column].colour = colour;
    this->board[0][column].cellType = type;
    this->fall(0, column);
}

void TextView:: fall(int r, int c) {
    if (this->board[r][c].colour == Empty) {
        string("Error: Attempted to call view::fall on an Empty block: ") + to_string(r) + " " + to_string(c);
    }
    int i = r;
    while (i < this->size - 1 && this->board[i+1][c].colour == Empty) {i++;}
    _swap(this->board[r][c], this->board[i][c]);
};


void TextView:: destroy(int r, int c) {
    if (this->board[r][c].colour == Empty) {
        string("Error: Attempted to call view::destroy on an Empty block: ") + to_string(r) + " " + to_string(c);
    }
    this->board[r][c].colour = Empty;
    this->board[r][c].cellType = Basic;
}

void TextView:: destroy(vector<int> rows, vector<int> cols) {
    int limR = rows.size();
    int limC = cols.size();
    if (limC != limR) {throw string("ERROR: The vectors' sizes passed in View::destroy don't match: rows.size() = ") + to_string(limR) + ", cols.size() = " + to_string(limC);}
    for (int i = 0; i < limR; i++) {
        this->destroy(rows[i], cols[i]);
    }
};

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
    // system("tput setaf 1");
    cout << ">> " << str << endl;
    // system("tput setaf 0");
}

void TextView:: setLabel(const string& str) {
    this->customLabel = str;
}
