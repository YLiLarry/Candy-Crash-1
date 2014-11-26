#include <iostream>
#include "controller/game.h"
using namespace std;

int main() {
	Game game;
    #if DEBUG
        int i;
        cerr << "Size: ";
        cin >> i;
        game.start(i);
    #else
       game.start(10);
    #endif
}
