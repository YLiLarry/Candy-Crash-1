/*
    Note that the Model should NOT hold any pointer to TextView.
    Model should hold pointers to View (the super abstract class) instead.
*/

#ifndef __TEXTVIEW__H__
#define __TEXTVIEW__H__
#include <vector>
#include "../view.h"
#include "../cellview.h"    
    
    class TextCell : public CellView {
        public:
            void draw() const;
    };
    
    class TextView : public View {
        protected:
            TextCell** board;
            void init();
            
        public:
            TextView(int);
            ~TextView();
            
            void init(int);
            void draw() const;
            
            void setColour(int,int,Colour);
            void setType(int,int,Type);
            // void set(std::vector<Cell*>); // requires the Cell class definition
            void setScore(int);
            void setLevel(int);
            void setMovesRemain(int);
            void setHiScore(int);
            
            void swap(int,int,Direction);
            void drop(int,Colour,Type);
            void fall(int,int,int);
            void destory(int,int);
            
            void end();
            void restart(int);
    };

#endif
