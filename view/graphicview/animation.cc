#include <iostream>
#include <exception>
#include "animation.h"
using namespace std;

void Animation:: loop() {
    if ((! this->toggle) || this->queue.empty()) {
        this->toggle = false;
        return;
    }
    if (this->during(this->queue.front())) {
        this->animate(this->queue.front());
        this_thread::sleep_for(this->fps);
    } else {
        // #if DEBUG_GRAPHIC
        //     fprintf(stderr,"Animation pop, after pop: %lu\n", this->queue.size());
        // #endif
        this->queue.pop_front();
        while (true) {this_thread:: yield(); break;}
        // #if DEBUG_GRAPHIC
        //     fprintf(stderr,"!! THREAD END\n");
        // #endif
    }
    this->loop();
}

void Animation:: end() {this->toggle = false;}
bool Animation:: done() {return ! this->toggle;}
void Animation:: join() {
    if (this->td && this->td->joinable()) {
        this->td->join();
    }
}

void Animation:: push(std::vector<int> c) {
    // #if DEBUG_GRAPHIC
    //     fprintf(stderr,"Animation push %d %d\n", c[0], c[1]);
    // #endif
    this->queue.push_back(c);
    // #if DEBUG_GRAPHIC
    //     fprintf(stderr,"Animation after push %lu\n", this->queue.size());
    // #endif
    if (this->toggle == false) {
        this->toggle = true;
        // #if DEBUG_GRAPHIC
        //     fprintf(stderr,"Animation detach\n");
        // #endif
        this->td = new thread(&Animation::loop, this);
    }
}

Animation:: ~Animation() {
    if (! this->td) {return;}
    this->td->join();
    this->end();
    delete this->td;
}

Animation:: Animation(std::chrono::milliseconds fps) {
    this->toggle = false;
    this->td = NULL;
    this->fps = fps;
}
