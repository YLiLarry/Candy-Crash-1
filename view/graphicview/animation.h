#ifndef __H__ANIMATE__
#define __H__ANIMATE__
#include <thread>
#include <chrono>
#include <list>
#include <vector>
class Animation {
    
    protected :
    
    private :
        void loop();
        bool toggle = false; // true is not threadsafe.
        std:: thread* td = NULL;
        std::list<std::vector<int>> queue; // dont trust
        const std::chrono::milliseconds fps = std::chrono::milliseconds(1000/50);
        
    public :
        void end();
        bool done();
        void join();
        void push(std::vector<int>);
        virtual bool during(std::vector<int>) = 0;
        virtual void animate(std::vector<int>) = 0;
        virtual ~Animation();
};
#endif
