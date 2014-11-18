#ifndef __VIEW__H__
#define __VIEW__H__
    class View {
        Model* model;
        void update() const;
        void draw() const;
    };
#endif
