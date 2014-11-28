/*
    An super abstract class for GraphicView & TextView.
    The Model class should hold pointers to this class for polymorphism.
*/

#ifndef __VIEWINTERFACE__H__
#define __VIEWINTERFACE__H__
#include "../public/global.h"
#include <string>
    
    class CellViewAbstract {
        public:
            Colour colour;
            Type cellType;
            virtual void draw() const = 0;
            virtual ~CellViewAbstract() {};
    };

    class ViewAbstract {
        
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
            CellViewAbstract** board;
            virtual void init(int) = 0;
            virtual void end() = 0;
            
        public:
            // View(int);
            virtual ~ViewAbstract() {};
            
            virtual void draw() const = 0; 
            
            /* setters */
            virtual void setColour(int,int,Colour) = 0;
            virtual void setType(int,int,Type) = 0;

            virtual void setScore(int) = 0;
            virtual void setLevel(int) = 0;
            virtual void setMovesRemain(int) = 0;
            virtual void setHiScore(int) = 0;
            
            virtual void print(const std::string&) = 0;
            virtual void swap(int,int,Direction) = 0;
            virtual void drop(int,Colour,Type) = 0;
            virtual void fall(int,int,int) = 0;
            virtual void destory(int,int) = 0;
            virtual void restart(int) = 0;

    };
    
#endif
