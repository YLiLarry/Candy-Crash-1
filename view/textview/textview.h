/*
    Note that the Model should NOT hold any pointer to TextView.
    Model should hold pointers to View (the super abstract class) instead.
*/

#ifndef __TEXTVIEW__H__
#define __TEXTVIEW__H__
#include <vector>
#include "../viewabstract.h"
    
    class TextCell : public CellViewAbstract {
        public:
            void draw();
    };
    
    class TextView : public ViewAbstract {
        protected:
            TextCell** board;
            void init();
            
        public:
            TextView(int);
            ~TextView();
            
            void init(int);
            void draw();
            
            void setColour(int,int,Colour);
            void setType(int,int,Type);
            void setScore(int);
            void setLevel(int);
            void setLocked(int,int,bool);
            void setMovesRemain(int);
            void setHiScore(int);
            
            void print(const std::string&);
            void setLabel(const std::string&);
            void swap(int,int,Direction);
            void drop(int,Colour,Type);
            void fall(int,int);
            void destroy(int,int);
            void destroy(std::vector<int>,std::vector<int>);
            
            void end();
            void restart(int);
    };

#endif
