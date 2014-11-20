/*
    Note that the Model should NOT hold any pointer to GraphicView.
    Model should hold pointers to View (the super abstract class) instead.
*/

#ifndef __GRAPHICVIEW__H__
#define __GRAPHICVIEW__H__

#include "view.h"

    class GraphicCell : public CellView {
        protected:
            int x;
            int y;
        public:
            GraphicCell(int,int);
            ~GraphicCell();
    }

    class GraphicView : public View {
        protected:
            
    };
    
#endif
