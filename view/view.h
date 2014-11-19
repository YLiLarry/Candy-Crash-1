/*
    An super abstract class for GraphicView & TextView.
    The Model class should hold pointers to this class for polymorphism.
*/
    
/*
    I just realized that since the GraphicView (if there's animation) should calls draw() on itself in a seperate thread 
    with a fixed FPS, the TextView better calls draw() on itself too. That is, when you call `swap()` on both View, 
    they change the positions of cells and display automatically. Only the `init()` method should not call `draw()`.
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
        public:
            virtual void draw() const = 0;
            
        public:
            // View(int);
            virtual ~View() {};
            /* setters */
            virtual void setColour(int,int,COLOUR) = 0;
            virtual void setType(int,int,CELLTYPE) = 0;
            virtual void set(std::vector<Cell*>) = 0; // requires the Cell class definition
            virtual void setScore(int) = 0;
            virtual void setLevel(int) = 0;
            virtual void setMovesRemain(int) = 0;
            virtual void setHiScore(int) = 0;
            
            virtual void swap(int,int,DIRECTION) = 0;
            virtual void destory(int,int) = 0;
            virtual void destory(std::vector<Cell*>) = 0; // requires the Cell class definition

    };
    
#endif
