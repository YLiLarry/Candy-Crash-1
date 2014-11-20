#ifndef __CELLVIEW__H__
#define __CELLVIEW__H__
#include "../model/cell.h"

    class CellView {
        public:
            Colour colour;
            CellType cellType;
            CellView();
            virtual ~CellView() {};
            virtual void draw() const = 0;
            bool empty() const;
            void setEmpty();
    };
    
#endif
