/*
    Note that the Model should NOT hold any pointer to GraphicView.
    Model should hold pointers to View (the super abstract class) instead.
*/

#ifndef __GRAPHICVIEW__H__
#define __GRAPHICVIEW__H__

#include "../viewabstract.h"
#include "window.h"
#include "animation.h"
#include <thread>
#include <chrono>
    
    class GraphicCell;
    class GraphicView;

    class MoveAnimation : public Animation {
        private :
            GraphicCell* target;
            int desX;
            int desY;
        public :
            bool during();
            void animate();
            void to(int,int);
            MoveAnimation(GraphicCell*);
    };

    class GraphicCell : public CellViewAbstract {
        public:
            int x;
            int y;
            Xwindow* window;
            Colour colour;
            GraphicView* outer;
            MoveAnimation* move;
            
            GraphicCell();
            void draw() const;
    };

    class GraphicView : public ViewAbstract {
        
        public :
            GraphicCell*** board;
            Xwindow* window;
            int windowWidth;
            int windowHeight;
            int cellSize;
            int marginLeft;
            int marginTop;
            int fps;
            std::thread* main;
            
            void refresh();
            
        public :
            GraphicView(int);
            ~GraphicView();
            
            void init(int);
            void draw() const;
            
            void setColour(int,int,Colour);
            void setType(int,int,Type);
            void setScore(int);
            void setLevel(int);
            void setMovesRemain(int);
            void setHiScore(int);
            
            void print(const std::string&);
            void setLabel(const std::string&);
            void swap(int,int,Direction);
            void drop(int,Colour,Type);
            void fall(int,int);
            void destroy(int,int);
            
            void end();
            void restart(int);
    };
    
#endif
