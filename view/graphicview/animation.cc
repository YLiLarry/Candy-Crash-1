#include <iostream>
#include <exception>
#include "animation.h"
using namespace std;

void Animation:: loop() {
    if (this->toggle && this->during()) {
        animate();
        this_thread::sleep_for(chrono::milliseconds(this->fps));
        this->loop();
    }
}

void Animation:: end() {
    // cerr << "td: " << this->td << endl;
    if (this->td) {
        // cerr << "td joinable?" << this->td->joinable() << endl;
        this->toggle = false;
        this->td->join();
        // cerr << "td join!" << endl;
        delete this->td;
    }
}

void Animation:: start() {
    cerr << "start" << endl;
    this->end();
    toggle = true;
    this->td = new thread(&Animation::loop, this);
}

Animation:: ~Animation() {
    cerr << "Animation destroied." << endl;
    this->end();
}

bool Move:: during() {
    cerr << target << " " << current << " ";
    return target > current;
}

void Move:: animate() {
    cerr << this->current++ << endl;
}

// int main() {
//     Move m;
//     m.start();
//     this_thread::sleep_for(chrono::seconds(1));
//     cerr << "main quit" << endl;
// }
