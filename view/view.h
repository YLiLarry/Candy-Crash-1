/*
    An super abstract class for GraphicView & TextView.
    The Model class should hold pointers to this class for polymorphism.
*/
    
/*
    I just realized that since the GraphicView (if there's animation) should calls draw() on itself in a seperate thread 
    with a fixed FPS, the TextView better calls draw() on itself too. That is, when you call `sawp()` on both View, 
    they change the positions of cells and display automatically. Only the `init()` method should not call `draw()`.
*/

#ifndef __VIEW__H__
#define __VIEW__H__
#include "../model/model.h"
    
    class CellView {
        
        public:
            int x;
            int y;
            COLOUR colour;
            virtual void moveUp() = 0;
            virtual void moveDown() = 0;
            virtual void moveLeft() = 0;
            virtual void moveRight() = 0;
            virtual void destory() = 0;
            
    };
    
    class View {

        protected:
            Model* model;
            unsigned int m_score;
            unsigned int m_movesRemain;
            unsigned int m_level;
            unsigned int m_hiScore;
            std::vector< std::vector<CellView> > board;
            virtual void draw() const = 0;
            
        public:
            View(Model*, int);
            ~View();
            virtual void init() = 0;
            virtual void init(std::vector<Cell*>) = 0; // overloaded
            virtual void score() = 0;
            virtual void level() = 0;
            virtual void movesRemain() = 0;
            virtual void hiScore() = 0;
            
            virtual void sawp(int,int,COLOUR) = 0;
            virtual void clearMatched(int,int) = 0;
            virtual void clearMatched(std::vector<Cell*>) = 0; // overloaded
            
    };
    
#endif
