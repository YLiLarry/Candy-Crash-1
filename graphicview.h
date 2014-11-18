/*
    Note that the Model should NOT hold any pointer to GraphicView.
    Model should hold pointers to View (the super abstract class) instead.
*/

#ifndef __GRAPHICVIEW__H__
#define __GRAPHICVIEW__H__

#include "view.h"

    class GraphicView : public View {
        protected:
            int mapX(int);
            int mapY(int);
    };
    
#endif
