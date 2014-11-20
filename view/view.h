/*
    An super abstract class for GraphicView & TextView.
    The Model class should hold pointers to this class for polymorphism.
*/

#ifndef __VIEW__H__
#define __VIEW__H__
#include "../public/global.h"
#include "cellview.h"
    
    class prepare;
    class fall;
    class destory;

    class View {
        
        protected:
            
        #if DEBUG_VIEW
            public:
        #endif
                
            int score;
            int movesRemain;
            int level;
            int hiScore;
            int canvasWidth;
            int cellWidth;
            int size;
            CellView** board;
            
        public:
            // View(int);
            virtual ~View() {};
            
            virtual void init(int) = 0;
            virtual void draw() const = 0; 
            
            /* setters */
            virtual void setColour(int,int,Colour) = 0;
            virtual void setType(int,int,CellType) = 0;

            virtual void setScore(int) = 0;
            virtual void setLevel(int) = 0;
            virtual void setMovesRemain(int) = 0;
            virtual void setHiScore(int) = 0;
            
            virtual void swap(int,int,Direction) = 0;
            virtual void drop(int,Colour,CellType) = 0;
            virtual void fall(int,int,int) = 0;
            virtual void destory(int,int) = 0;
            virtual void restart(int) = 0;

    };
    
#endif
