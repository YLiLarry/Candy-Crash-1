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
#include <vector>
    
    class GraphicCell;
    class GraphicView;

    class CellAnimation : public Animation {
        protected :
            GraphicCell* target;
        public :
            CellAnimation() {};
            CellAnimation(GraphicCell* gc) : target(gc) {};
    };

    class MoveAnimation : public CellAnimation {
        public :
            bool during(std::vector<int>);
            void animate(std::vector<int>);
            void to(int,int);
            MoveAnimation() {};
            MoveAnimation(GraphicCell* gc) : CellAnimation(gc) {};
    };
    
    class FallAnimation : public CellAnimation {
        public :
            bool during(std::vector<int>);
            void animate(std::vector<int>);
            void to(int, int);
            FallAnimation() {};
            FallAnimation(GraphicCell* gc) : CellAnimation(gc) {};
    };

    class GraphicCell : public CellViewAbstract {
        public:
            int x;
            int y;
            int lx;
            int ly;
            int speed;
            bool needDraw;
            Colour colour;
            Xwindow* window;
            GraphicView* outer;
            
            MoveAnimation move;
            FallAnimation fall;
            
            GraphicCell();
            void draw();
    };

    class GraphicView : public ViewAbstract {
        
        private :
            bool toggle;
            std::vector<int> droppingNum;
            std::thread* td;
        
        public :
            GraphicCell*** board;
            Xwindow* window;
            int windowWidth;
            int windowHeight;
            int cellSize;
            int marginLeft;
            int marginTop;
            std::chrono::milliseconds fps;
            
            void refresh();
            
        public :
            GraphicView(int);
            ~GraphicView();
            
            void waitAllAnimationsFinish();
            
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
            
            void end();
            void restart(int);
    };
    
#endif
