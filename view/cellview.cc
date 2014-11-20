#include "cellview.h"
CellView:: CellView() : colour(White), cellType(Basic) {}

bool CellView:: empty() const {return (this->colour == Empty);}
void CellView:: setEmpty() {this->colour = Empty;}
