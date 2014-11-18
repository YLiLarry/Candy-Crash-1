/*
    Note that the Model should NOT hold any pointer to TextView.
    Model should hold pointers to View (the super abstract class) instead.
*/

#ifndef __TEXTVIEW__H__
#define __TEXTVIEW__H__
#include <vector>
#include "view.h"

    class TextView : public View {
        private:
            std::vector<char>  board;
        public:
            TextView(Model*);
            ~TextView();
            void draw() const;
            void update();
    };

#endif


View:: hey();
