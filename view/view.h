/*
    An super abstract class for GraphicView & TextView.
    The Model class should hold pointers to this class for polymorphism.
*/

#ifndef __VIEW__H__
#define __VIEW__H__
#include "viewabstract.h"
#include "textview/textview.h"
#include "graphicview/graphicview.h"

    class View : public ViewAbstract {
        private:
            TextView* tv;
            GraphicView* gv;
            
        public:
            View(int);
            ~View();

            void init(int);
            void draw() const;
            
            void setColour(int,int,Colour);
            void setType(int,int,Type);
            void setScore(int);
            void setLevel(int);
            void setMovesRemain(int);
            void setHiScore(int);
            
            void print(const std::string&);
            void swap(int,int,Direction);
            void drop(int,Colour,Type);
            void fall(int,int,int);
            void destory(int,int);
            
            void end();
            void restart(int);
    };
    
#endif
