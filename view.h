#ifndef __VIEW__H__
#define __VIEW__H__

    class View {
        protected:
            Model* model;
        public:
            void update() const;
            void draw() const;
    };
    
#endif
