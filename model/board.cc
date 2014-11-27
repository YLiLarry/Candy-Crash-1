#include <fstream>
#include <sstream>
#include <cmath>
#include "board.h"
#include "../public/global.h"
#include "PRNG.h"

using namespace std;

Board::Board(int n) {

	view = new View(n);
	grid = new Square *[n];
	for (int r = 0; r < n; r++) {
		grid[r] = new Square[n];
		for (int c = 0; c < n; c++) {
			grid[r][c].init(r, c, n, grid, view);
		}
	}

	size = n;

	level = 0;
	score = 0;
	turnScore = 0;

}

Board::~Board() {

	for (int i = 0; i < size; i++) {

		delete[] grid[i];
	}

	delete[] grid;

	delete view;
}

void Board::start() {
	loadLevel(level);
}

void Board::loadLevel(int level) {

	if (level == 0) {

		#ifdef DEBUG
				cerr << "File: ";
		
				string f;
				cin >> f;
		
				ifstream file(f.c_str());
				if (! file.good()) {throw string("unable to read the sequence file: '") + f + "'";}
		#else
				ifstream file("sequence.txt");
				if (! file.good()) {throw string("unable to read the sequence.txt file");}
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
					default: {throw string("unexpected square type: '") + square[1] + "'";}
				}

				Colour colour = (Colour)(square[2] - '0');

				grid[i][j].setColour(colour);
				grid[i][j].setType(type);
				grid[i][j].setNeighbours();

				if (i == size - 1 && j == size - 1) {

					file >> levelZeroColours;
				}
			}
		}
		
		view->setScore(score);
		view->setLevel(level);
	}
	
	view->draw();
}

void Board::setNewSquare(Square &sq) {

	if (level == 0) {

		Colour newColour = (Colour)(levelZeroColours[0] - '0');
		Type newType = Basic;

		sq.setColour(newColour);
		sq.setType(newType);

		// recycles the colours
		char c = levelZeroColours[0];
		levelZeroColours.erase(0, 1);
		levelZeroColours += c;
	}
}

void Board::swap(int row, int col, Direction d) {

	cleared = 0;
	turnScore = 0;
	chain = 0;
	chainMode = false;
	settled = false;

	grid[row][col].swapWith(d);
	
	view->draw();

	clearAt(*grid[row][col].neighbour[d]);
	clearAt(grid[row][col]);

	if (!cleared) grid[row][col].swapWith(d);

	grid[row][col].clearNotified();

	view->draw(); // print before dropping

	while (!settled) {
		// view->draw(); // temp
		dropSquares();
		// view->draw(); // temp
		chainReaction();
	}

	dropSquares();

	view->draw();

	score += turnScore;

	ostringstream ss;
	ss << "cleared:  " << cleared << endl;
	ss << "chained:  " << chain << endl;
	ss << "score  : +" << turnScore << endl;
	ss << "total  :  " << score << endl;

	#if ! DEBUG
		view->print(ss.str());
	#endif
}

void Board::dropSquares() {

	for (int c = 0; c < size; c++) {
		grid[0][c].drop();

		while (grid[0][c].getColour() == Empty) {

			setNewSquare(grid[0][c]);
			grid[0][c].drop();
		}
	}
}

void Board::chainReaction() {

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			settled = true;

			grid[r][c].notify();

			if (grid[r][c].isReady()) {

				settled = false;
				chainMode = true;

				view->draw();
				clear(grid[r][c], 4);
			}

			grid[r][c].clearNotified();
		}
	}
}

int Board::clearAt(Square &root) {

	collectMatched(root);

	Colour backup = root.getColour();
	int radius = 0;

	if (hMatch.size() > 3 || vMatch.size() > 3) {

		radius = 4;
	} else if (hMatch.size() == 3 || vMatch.size() == 3) {

		radius = 2;
	}

	if (hMatch.size() < 3 && vMatch.size() < 3) {

		// view->print("no match");
		return false;

	} else if (hMatch.size() == 3 && vMatch.size() == 3) {

		// view->print("L match");

		for (int i = 0; i < 3; i++) {
			clear(*hMatch[i], radius);
		}
		for (int i = 0; i < 3; i++) {
			clear(*vMatch[i], radius);
		}

		root.setColour(backup);
		root.setType(Unstable);

	} else if (hMatch.size() > vMatch.size()) {

		// view->print("Horizontal match");

		int n = (int)hMatch.size();

		for (int i = 0; i < n; i++) {
			clear(*hMatch[i], radius);
		}

		switch (n) {
			case 4: root.setColour(backup);
					root.setType(Lateral); 
					break;
			case 5: root.setColour(backup);
					root.setType(Psychedelic);
					break;
		}

	} else if (hMatch.size() < vMatch.size()) {

		// view->print("Vertical match");

		int n = (int)vMatch.size();

		for (int i = 0; i < n; i++) {
			clear(*vMatch[i], radius);
		}

		switch (n) {
			case 4: root.setColour(backup);
					root.setType(Upright); 
					break;
			case 5: root.setColour(backup);
					root.setType(Psychedelic);
					break;
		}
	}

	return true;
}

void Board::clear(Square &sq, int r) {

	if (sq.getColour() == Empty)  return;

	Colour tColour = sq.getColour();
	Type tType = sq.getType();

	sq.setColour(Empty);
	sq.setType(Basic);
	sq.setReady(false);

	cleared++;

	if (cleared > 3) r = 4; // override;

	if (chainMode) {

			chain++;
			turnScore *= pow(2, chain);
	} else {

		switch (cleared) {
			case 0: case 1: case 2: break;
			case 3: turnScore = 3; break;
			case 4: turnScore = 8; break;
			case 5: turnScore = 15; break;
			default: turnScore = 4 * cleared;
		}
	}

	int row = sq.getRow();
	int col = sq.getCol();

	switch (tType) {

		case Basic: break;
		case Lateral: 
		{
			for (int c = 0; c < size; c++) {
				clear(grid[row][c], r);
				//grid[row][c].clear(cleared, chain, turnScore, r);
			}					  
		} break;			  
		case Upright:
		{
			for (int r = 0; r < size; r++) {
				clear(grid[r][col], r);
				//grid[r][col].clear(cleared, chain, turnScore, r);
			}
		} break;
		case Unstable:
		{
			int sz = size; // looks pretty

			int rMin = (row - r >= 0)? row - r : 0;
			int rMax = (row + r < sz)? row + r : sz - 1;
			int cMin = (col - r >= 0)? col - r : 0;
			int cMax = (col + r < sz)? col + r : sz - 1;

			for (int r = rMin; r <= rMax; r++) {
				for (int c = cMin; c <= cMax; c++) {
					clear(grid[r][c], r);
					//grid[r][c].clear(cleared, chain, turnScore, r);
				}
			}	   
		} break;
		case Psychedelic:
		{
			for (int r = 0; r < size; r++) {
				for (int c = 0; c < size; c++) {
					if (grid[r][c].getColour() == tColour) {
						clear(grid[r][c], r);
						//grid[i][j].clear(cleared, chain, turnScore, r);
					}
				}
			}
		} break;
	}
}

void Board::collectMatched(Square &root) {

	hMatch.clear();
	vMatch.clear();

	int row = root.getRow();
	int col = root.getCol();

	// self
	if (grid[row][col].isReady()) {
		hMatch.push_back(&root);
		vMatch.push_back(&root);
	}
	// up
	for (int r = row - 1; r >= 0; r--) {
		if (grid[r][col].isReady()) {
			vMatch.push_back(&grid[r][col]);
		}
	}
	// down
	for (int r = row + 1; r < size; r++) {
		if (grid[r][col].isReady()) {
			vMatch.push_back(&grid[r][col]);
		}
	}
	// left
	for (int c = col - 1; c >= 0; c--) {
		if (grid[row][c].isReady()) {
			hMatch.push_back(&grid[row][c]);
		}
	}
	// right
	for (int c = col + 1; c < size; c++) {
		if (grid[row][c].isReady()) {
			hMatch.push_back(&grid[row][c]);
		}
	}
}

string Board::validMove() {

	ostringstream ss;
	bool foundMatch = false;

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {
			for (int d = 0; d < 4; d++) {

				if (grid[r][c].neighbour[d]) {
					grid[r][c].swap((Direction)d);

					if (grid[r][c].isReady() ||
						grid[r][c].neighbour[d]->isReady()) {
						ss << "hint: " << r << " " << c << " " << dir2str((Direction)d);
						foundMatch = true;
					}

					grid[r][c].clearReady();
					grid[r][c].clearNotified();

					grid[r][c].swap((Direction)d);

					grid[r][c].clearReady();
					grid[r][c].clearNotified();

					if (foundMatch) return ss.str();
				}
			}
		}
	}

	ss << "none";
	return ss.str();
}

void Board::hint() {
	view->print(validMove());
}

void Board::scramble() {

	if (validMove() != "none") return;

	PRNG rand;
	
	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			int randRow = rand(0, size-1);
			int randCol = rand(0, size-1);

			grid[r][c].swap(grid[randRow][randCol]);

			if (grid[r][c].isReady() ||
				grid[randRow][randCol].isReady()) {

				grid[r][c].swap(grid[randRow][randCol]);
			}

			grid[r][c].clearReady();
			grid[r][c].clearNotified();

			grid[randRow][randCol].clearReady();
			grid[randRow][randCol].clearNotified();

			view->setColour(r, c, grid[r][c].getColour());
			view->setType(r, c, grid[r][c].getType());

			view->setColour(randRow, randCol, grid[randRow][randCol].getColour());
			view->setType(randRow, randCol, grid[randRow][randCol].getType());
		}
	}

	view->draw();
}


void Board::printGridInfo() {

	for (int i = 0; i < size; i++) {

		for (int j = 0; j < size; j++) {

			cerr << grid[i][j].isNotified() << grid[i][j].isReady() << " ";
		}
		cerr << endl;
	}
}

void Board::levelUp() {this->loadLevel(++this->level);}
void Board::levelDown() {this->loadLevel(--this->level);}
void Board::restart() {this->loadLevel(this->level);}
