/*
    An super abstract class for GraphicView & TextView.
    The Model class should hold pointers to this class for polymorphism.
*/

#ifndef __VIEW__H__
#define __VIEW__H__
#include "../Model/model.h"
    class View {
        /* expecting :
            model->player->score
        */
        protected:
            Model* model;
            unsigned int score;
            unsigned int movesLeft;
            virtual void draw() const = 0;
            
        public:
            View(Model*, int);
            ~View();
            virtual void setScore() = 0;
    };
    
#endif
