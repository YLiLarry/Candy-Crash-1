#ifndef __H__ANIMATE__
#define __H__ANIMATE__
#include <thread>
#include <chrono>
class Animation {
    private :
        std::thread* td = 0;
    protected :
        void loop();
        bool virtual during() = 0;
        void virtual animate() = 0;
        virtual Animation* getInstance() = 0;
    public :
        void end();
        void run();
        virtual ~Animation();
};
class Move : public Animation {
    int target = 10;
    int current = 0;
    bool during();
    void animate();
    Move* getInstance();
    
    public :
    ~Move();
};
#endif
