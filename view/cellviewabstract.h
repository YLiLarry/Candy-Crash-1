#ifndef __CELLVIEW__H__
#define __CELLVIEW__H__
    #include "../public/global.h"
    class CellViewAbstract {
        public:
            Colour colour;
            Type cellType;
            CellViewAbstract();
            virtual ~CellViewAbstract() {};
            virtual void draw() const = 0;
            bool empty() const;
            void setEmpty();
    };
    
#endif
