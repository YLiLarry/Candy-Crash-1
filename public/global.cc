#include "global.h"
std::string dir2str(Direction d) {
    switch (d) {
        case Up: return "up";
        case Down: return "down";
        case Left: return "left";
        case Right: return "right";
    }
}
