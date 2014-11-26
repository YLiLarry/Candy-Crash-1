#include <iostream>
#include "controller/game.h"
using namespace std;

int main() {
    try {
        #if DEBUG
            int i;
            cerr << "Size: ";
            cin >> i;
            Game game(i);
        #else
            Game game(10);
        #endif
    } catch (const string& e) {
        cerr << e << endl;
    }
}
