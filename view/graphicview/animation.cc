#include <iostream>
#include <exception>
#include "animation.h"
using namespace std;

void Animation:: loop() {
    if (this->during()) {
        animate();
        this_thread::sleep_for(chrono::milliseconds(500));
        this->loop();
    }
}

void Animation:: end() {
    cerr << "td: " << this->td << endl;
    if (this->td) {
        cerr << "td joinable?" << this->td->joinable() << endl;
        this->td->join();
        cerr << "td join!" << endl;
        // delete this->td;
    }
}

void Animation:: run() {
    cerr << "run" << endl;
    this->end();
    this->td = new thread(&Animation::loop, getInstance());
}

Animation:: ~Animation() {
    cerr << "Animation destroied." << endl;
    delete this->td;
}

bool Move:: during() {
    cerr << target << " " << current << " ";
    return target > current;
}

void Move:: animate() {
    cerr << this->current++ << endl;
}

Move* Move:: getInstance() {return this;}
Move:: ~Move() {
    cerr << "Move destroied" << endl;
}

int main() {
    Move m;
    m.run();
    this_thread::sleep_for(chrono::seconds(1));
    cerr << "main quit" << endl;
    m.end();
}
