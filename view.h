/*
    An super abstract class for GraphicView & TextView.
    The Model class should hold pointers to this class for polymorphism.
*/

#ifndef __VIEW__H__
#define __VIEW__H__

    class View {
        protected:
            Model* model;
        public:
            virtual void update() const = 0;
            virtual void draw() const = 0;
            View();
            ~View();
    };
    
#endif
