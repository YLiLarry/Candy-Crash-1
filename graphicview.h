#ifndef __GRAPHICVIEW__H__
#define __GRAPHICVIEW__H__

#include "view.h"

    class GraphicView : public View {
        protected:
            int mapX(int);
            int mapY(int);
    };
    
#endif
