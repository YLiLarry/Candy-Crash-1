#include "square.h"

Square::Square() {
	row = 0;
	col = 0;
	colour = Empty;
	type = Basic;

	for (int i = 0; i < 4; i++) {
		neighbours[i] = NULL;
	}
}
