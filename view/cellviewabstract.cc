#include "cellviewabstract.h"
CellViewAbstract:: CellViewAbstract() : colour(White), cellType(Basic) {}

bool CellViewAbstract:: empty() const {return (this->colour == Empty);}
void CellViewAbstract:: setEmpty() {this->colour = Empty;}
