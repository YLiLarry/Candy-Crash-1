#ifndef __CELL_H__
#define __CELL_H__

    /* Abstract */
    class Cell {
        protected:
            Cell* cell;
    };
    
    class Lateral : public Cell {};
    
    class Upright : public Cell {};
    
    class Unstable : public Cell {};
    
    class Psychedelic : public Cell {};
    
	enum Celltype {Basic = 0, Lateral, Upright, Unstable, Psychedelic};
	//enum COLOUR {WHITE, BLUE, GREEN, RED, EMPTY};
	
#endif
