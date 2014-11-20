#include "cellview.h"
CellView:: CellView() : colour(WHITE), cellType(BASIC) {}

bool CellView:: empty() const {return (this->colour == EMPTY);}
void CellView:: setEmpty() {this->colour = EMPTY;}
