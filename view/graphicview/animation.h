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
        bool toggle; // true is not threadsafe.
        std:: thread* td;
        std::list<std::vector<int>> queue; // dont trust
        std::chrono::milliseconds fps;
        
    public :
        void end();
        bool done();
        void join();
        void push(std::vector<int>);
        virtual bool during(std::vector<int>) = 0;
        virtual void animate(std::vector<int>) = 0;
        Animation();
        virtual ~Animation();
};
#endif
