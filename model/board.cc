#include <fstream>
#include "board.h"
#include "../view/textview/textview.h"
#include "../public/global.h"

using namespace std;

Board::Board(int n) : size(n) {
	// initializing grid
	grid = new Square *[size];
	for (int i = 0; i < size; i++) {
		grid[i] = new Square[size];
	}

	level = 0;
	score = 0;

	view = new View(size);
	loadLevel(level);
	view->draw();
}

Board::~Board() {
	for (int i = 0; i < size; i++) {
		delete[] grid[i];
	}
	delete[] grid;

	delete view;
}

void Board::loadLevel(int level) {
	if (level == 0) {

#ifdef MATCH_TEST
		cerr << "file name: ";
		string f;
		cin >> f;
		ifstream file(("levels/" + f).c_str());
#else
		ifstream file("sequence.txt");
#endif


		string square;

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {

				file >> square;

				Type type;

				switch (square[1]) {
					case '_': type = Basic; break;
					case 'h': type = Lateral; break;
					case 'v': type = Upright; break;
					case 'b': type = Unstable; break;
					case 'p': type = Psychedelic; break;
				}

				Colour colour = (Colour)(square[2] - '0');

				grid[i][j].row = i;
				grid[i][j].col = j;
				grid[i][j].colour = colour;
				grid[i][j].type = type;
				grid[i][j].view = view;

				// setting neighbours
				
				if (i - 1 >= 0) 
					grid[i][j].neighbour[Up] = &grid[i - 1][j];
				if (i + 1 < size) 
					grid[i][j].neighbour[Down] = &grid[i + 1][j];
				if (j - 1 >= 0) 
					grid[i][j].neighbour[Left] = &grid[i][j - 1];
				if (j + 1 < size) 
					grid[i][j].neighbour[Right] = &grid[i][j + 1];

				view->setColour(i, j, colour);
				view->setType(i, j, type);
				view->setScore(score);
				view->setLevel(level);
			}
		}
	}
}

void Board::swap(int row, int col, Direction d) {

	grid[row][col].swapWith(d);
	view->draw();

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {

			cerr << grid[i][j].ready << " ";
		}
		cerr << endl;
	}
}

vector<Square *> Board::findMatches(int row, int col) {
	vector<Square *> matched;

	Colour colour = grid[row][col].colour;
	//Type type = swapped->type;

	// check horizontally for matching set of three
	for (int c = 0; c < size; c++) {
		if (c + 1 < size && c + 2 < size) {
			if (grid[row][c].colour == colour &&
					grid[row][c + 1].colour == colour &&
					grid[row][c + 2].colour == colour) {

				matched.push_back(&grid[row][c]);
				matched.push_back(&grid[row][c + 1]);
				matched.push_back(&grid[row][c + 2]);

				// add the rest (if any to matched vector)
				c++;
				while (grid[row][c].colour == colour) {
					matched.push_back(&grid[row][c]);
					c++;
				}
			}
		}
	}

	// check vertically for matching set of three
	for (int r = 0; r < size; r++) {
		if (r + 1 < size && r + 2 < size) {
			if (grid[r][col].colour == colour &&
					grid[r + 1][col].colour == colour &&
					grid[r + 2][col].colour == colour) {

				matched.push_back(&grid[r][col]);
				matched.push_back(&grid[r + 1][col]);
				matched.push_back(&grid[r + 2][col]);

				r++;
				while (grid[r][col].colour == colour) {
					matched.push_back(&grid[r][col]);
					r++;
				}
			}
		}
	}

	return matched;
}
