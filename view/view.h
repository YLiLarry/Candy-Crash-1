/*
    An super abstract class for GraphicView & TextView.
    The Model class should hold pointers to this class for polymorphism.
*/

#ifndef __VIEW__H__
#define __VIEW__H__
#include "../model/model.h"
#include "cellview.h"

    class View {
        
        protected:
            unsigned int score;
            unsigned int movesRemain;
            unsigned int level;
            unsigned int hiScore;
            unsigned int canvasWidth;
            unsigned int cellWidth;
            unsigned int size;
            CellView** board;
            
        #if DEBUG_VIEW
            public:
        #endif
            virtual void draw() const = 0; // DO NOT CALL DRAW() FROM MODEL
            
        public:
            // View(int);
            virtual ~View() {};
            /* setters */
            virtual void setColour(int,int,Colour) = 0;
            virtual void setType(int,int,Celltype) = 0;
            // virtual void set(std::vector<Cell*>) = 0; // requires the Cell class definition
            virtual void setScore(int) = 0;
            virtual void setLevel(int) = 0;
            virtual void setMovesRemain(int) = 0;
            virtual void setHiScore(int) = 0;
            
            virtual void swap(int,int,DIRECTION) = 0;
            // virtual void destory(int,int) = 0;
            // virtual void destory(std::vector<Cell*>) = 0; // requires the Cell class definition

    };
    
#endif
