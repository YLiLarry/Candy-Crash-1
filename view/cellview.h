#ifndef __CELLVIEW__H__
#define __CELLVIEW__H__
#include "../model/cell.h"

    class CellView {
        public:
            COLOUR colour;
            CELLTYPE cellType;
            CellView();
            virtual ~CellView() {};
            virtual void draw() = 0;
            virtual bool empty() = 0;
    };
    
#endif
