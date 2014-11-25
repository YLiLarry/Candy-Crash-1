#include "../board.h"
using namespace std;

Colour str2colour(string& str) {
    if (str == "_") {return Empty;}
    if (str == "0") {return White;}
    if (str == "1") {return Red;}
    if (str == "2") {return Green;}
    if (str == "3") {return Blue;}
    return White;
};

Direction str2dir(string& str) {
    if (str == "up") {return Up;}
    if (str == "down") {return Down;}
    if (str == "left") {return Left;}
    if (str == "right") {return Right;}
    return Up;
}

Type str2type(string& str) {
    if (str == "_") {return Basic;}
    if (str == "h") {return Lateral;}
    if (str == "v") {return Upright;}
    if (str == "b") {return Unstable;}
    if (str == "p") {return Psychedelic;}
    return Basic;
}

int main() {
    Board* board = NULL;
    int r, c;
    string str;
    
    while (cin >> str) {
        // for comment
        if (str == "//") {
            cin.ignore(1024, '\n');
            continue;
        } else
        if (str == "new") {
            cin >> r;
            if (board) {delete board;} // shoud not leak memory
            board = new Board(r);
            board->start();
        } else
        if (str == "hint") {
            board->hint();
        } else 
        if (str == "swap") {
            cin >> r >> c >> str;
            board->swap(r, c, str2dir(str));
            cout << endl;
        } else
        if (str == "restart") {
            // board->restart();
        } else
        if (str == "load") {
            board->loadLevel(0);
        } else
        // if (str == "ss") {
            // board->setScore(r);
        // } else 
        if (str == "quit") {
            break;
        }
    }
    
    delete board;
    return 0;
}
