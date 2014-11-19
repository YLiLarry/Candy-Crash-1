/*
    Note that the Model should NOT hold any pointer to TextView.
    Model should hold pointers to View (the super abstract class) instead.
*/

#ifndef __TEXTVIEW__H__
#define __TEXTVIEW__H__
#include <vector>
#include "../view.h"
#include "../cellview.h"    
    
    class CellTextView : public CellView {
        public:
            void draw() const;
            bool empty() const;
            void setEmpty();
    };
    
    class TextView : public View {
        protected:
            CellTextView** board;
            virtual void draw() const;
            
        public:
            TextView(int);
            ~TextView();
            
            void setColour(int,int,COLOUR);
            void setType(int,int,CELLTYPE);
            // void set(std::vector<Cell*>); // requires the Cell class definition
            void setScore(int);
            void setLevel(int);
            void setMovesRemain(int);
            void setHiScore(int);
            
            void swap(int,int,DIRECTION);
            void destory(int,int);
            // void destory(std::vector<Cell*>); // requires the Cell class definition
            void drop(int,COLOUR,CELLTYPE);
    };

#endif
