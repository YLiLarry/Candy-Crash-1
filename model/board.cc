#include <fstream>
#include <sstream>
#include <cmath>
#include "board.h"
#include "generator.h"
#include "../view/textview/textview.h"
#include "../public/global.h"
#include "PRNG.h"

using namespace std;

/*
 * Constructing a board for game of size n
 */
Board::Board(int n) {

	view = new View(n);
	generate = new Generator();
	grid = new Square *[n];

	for (int r = 0; r < n; r++) {

		// n coloumns
		grid[r] = new Square[n];

		for (int c = 0; c < n; c++) {

			// Initializing squares
			grid[r][c].init(r, c, n, grid, view);
		}
	}

	size = n;
	cleared = 0;
	chain = 0;
	unlocked = 0;

	score = 0;
	initScore = 0;
	matchScore = 0;
	turnScore = 0;

	level = 0;

	chainMode = false;
	emptyBoard = false;
}

/*
 * Board destructor
 */
Board::~Board() {

	for (int i = 0; i < size; i++) {

		delete[] grid[i];
	}

	delete[] grid;

	delete generate;
	delete view;
}

/*
 * Starts the game at level 0
 */
void Board::start() {
	loadLevel(level);
}

/*
 * parseSquare reads a string representation of a Square
 * and sets the Square as it is represented by the string
 * updates the view as well
 */
void parseSquare(string strSquare, Square &square, View *view) {

	// The locked status of the square
	bool locked = strSquare[0] == 'l';

	// THe colour of the square
	Colour colour = (Colour)(strSquare[2] - '0');

	// The type of the square
	Type type;
	switch (strSquare[1]) {
		case '_': type = Basic; break;
		case 'h': type = Lateral; break;
		case 'v': type = Upright; break;
		case 'b': type = Unstable; break;
		case 'p': type = Psychedelic; break;
		default: {throw string("unexpected square type: '") + strSquare[1] + "'";}
	}

	// Setting the actual scquare
	square.setLocked(locked);
	square.setColour(colour);
	square.setType(type);

	// Updating the view
	view->setLocked(square.getRow(), square.getCol(), locked);
	view->setColour(square.getRow(), square.getCol(), colour);
	view->setType(square.getRow(), square.getCol(), type);
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

				parseSquare(square, grid[i][j], view);
				grid[i][j].setNeighbours();

				if (i == size - 1 && j == size - 1) {

					file >> levelZeroColours;
				}
			}
		}
		
		view->setScore(score);
		view->setLevel(level);
		view->draw();

	} else if (level == 1) {

		for (int r = 0; r < size; r++) {
			for (int c = 0; c < size; c++) {

				parseSquare(generate->randomSquare(1), grid[r][c], view);
				grid[r][c].setNeighbours();
			}
		}

		scramble();

		view->setScore(score);
		view->setLevel(1);
		view->draw();
	
	} else if (level == 2) {

		generate->produced = 0;

		for (int r = 0; r < size; r++) {
			for (int c = 0; c < size; c++) {

				parseSquare(generate->randomSquare(2), grid[r][c], view);
				grid[r][c].setNeighbours();
			}
		}

		scramble();

		view->setScore(score);
		view->setLevel(2);
		view->draw();
	}
}

void Board::setNewSquare(Square &sq) {

	if (level == 0) {

		Colour newColour = (Colour)(levelZeroColours[0] - '0');
		Type newType = Basic;

		sq.setColour(newColour);
		sq.setType(newType);

		view->setColour(sq.getRow(), sq.getCol(), newColour);
		view->setType(sq.getRow(), sq.getCol(), newType);

		// recycles the colours
		char c = levelZeroColours[0];
		levelZeroColours.erase(0, 1);
		levelZeroColours += c;
	
	} else {

		parseSquare(generate->randomSquare(level), sq, view);
	}
}

void Board::swap(int row, int col, Direction d) {

	cleared = 0;
	turnScore = 0;
	chain = 0;
	chainMode = false;
	emptyBoard = false;

	grid[row][col].swapWith(d);
	
	view->draw();

	printGridInfo();

	clearAt(*grid[row][col].neighbour[d]);
	clearAt(grid[row][col]);

	if (!cleared) grid[row][col].swapWith(d);

	grid[row][col].clearNotified();

	do {
		view->draw();
		dropSquares();
		view->draw();
		chainReaction();

	} while (chainMode && !emptyBoard);

	score += turnScore;

	if (score >= initScore + 200 && level == 0) {
		
		level = 1;
		initScore = score;
	}

	if (score >= initScore + 300 && level == 1) {

		level = 2;
		initScore = score;
	}

	if (score >= initScore + 500 && level == 2 && unlocked == 20) {

		cerr << "WINNER!" << endl;
		return;
	}

	view->setScore(score);
	view->draw();

	ostringstream ss;
	ss << "cleared:  " << cleared << endl;
	ss << "chains :  " << chain << endl;
	ss << "scored : +" << turnScore << endl;
	ss << "total  :  " << score << endl;
	#if ! DEBUG
		view->print(ss.str());
	#endif
}

void Board::notifyAll() {

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			grid[r][c].notify();
		}
	}
}

void Board::unNotifyAll() {

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			grid[r][c].setNotified(false);
		}
	}
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

	chainMode = false;

	notifyAll();
	printGridInfo();

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			if (grid[r][c].isReady()) {

				if (!chainMode) chain++;
				chainMode = true;
				clear(grid[r][c], 4);
			}
		}
	}

	emptyBoard = true;

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			if (grid[r][c].getColour() != Empty) {
				emptyBoard = false;
				break;
			}
		}
	}

	unNotifyAll();
}

void Board::clearAt(Square &root) {

	collectMatched(root);

	Colour backup = root.getColour();
	int radius = 0;

	if (hMatch.size() > 3 || vMatch.size() > 3) {

		radius = 2;
	} else if (hMatch.size() == 3 || vMatch.size() == 3) {

		radius = 1;
	}

	if (hMatch.size() < 3 && vMatch.size() < 3) {

		// view->print("no match");
		return;

	} else if (hMatch.size() == 3 && vMatch.size() == 3) {

		for (int i = 0; i < 3; i++) {
			clear(*hMatch[i], radius);
		}
		for (int i = 0; i < 3; i++) {
			clear(*vMatch[i], radius);
		}

		root.setColour(backup);
		root.setType(Unstable);

		view->setColour(root.getRow(), root.getCol(), backup);
		view->setType(root.getRow(), root.getCol(), Unstable);

	} else if (hMatch.size() > vMatch.size()) {

		int n = (int)hMatch.size();

		for (int i = 0; i < n; i++) {
			clear(*hMatch[i], radius);
		}

		switch (n) {
			case 4: root.setColour(backup);
					root.setType(Lateral); 
					view->setColour(root.getRow(), root.getCol(), backup);
					view->setType(root.getRow(), root.getCol(), Lateral);
					break;
			case 5: root.setColour(backup);
					root.setType(Psychedelic);
					view->setColour(root.getRow(), root.getCol(), backup);
					view->setType(root.getRow(), root.getCol(), Psychedelic);
					break;
		}

	} else if (hMatch.size() < vMatch.size()) {

		int n = (int)vMatch.size();

		for (int i = 0; i < n; i++) {
			clear(*vMatch[i], radius);
		}

		switch (n) {
			case 4: root.setColour(backup);
					root.setType(Upright); 
					view->setColour(root.getRow(), root.getCol(), backup);
					view->setType(root.getRow(), root.getCol(), Upright);
					break;
			case 5: root.setColour(backup);
					root.setType(Psychedelic);
					view->setColour(root.getRow(), root.getCol(), backup);
					view->setType(root.getRow(), root.getCol(), Psychedelic);
					break;
		}
	}

	//view->draw();
}

void Board::clear(Square &sq, int r) {

	if (sq.getColour() == Empty)  return;

	if (sq.isLocked()) {

		sq.setLocked(false);
		view->setLocked(sq.getRow(), sq.getCol(), false);
		unlocked++;
		return;
	}

	Colour tColour = sq.getColour();
	Type tType = sq.getType();

	sq.setColour(Empty);
	sq.setType(Basic);
	sq.setReady(false);

	cleared++;

	if (cleared > 3) r = 2; // override;

	switch (cleared) {
		case 0: case 1: case 2: break;
		case 3: turnScore = 3; break;
		case 4: turnScore = 8; break;
		case 5: turnScore = 15; break;
		default: turnScore = 4 * cleared;
	}

	if (chainMode) turnScore = pow(2, chain) * turnScore;
	

	int row = sq.getRow();
	int col = sq.getCol();

	view->destroy(row, col);

	view->setScore(score + turnScore);
	view->draw();

	switch (tType) {

		case Basic: break;
		case Lateral: 
		{
			view->setLabel("Lateral");
			for (int c = 0; c < size; c++) {
				clear(grid[row][c], r);
			}					  
		} break;			  
		case Upright:
		{
			view->setLabel("Upright");
			for (int r = 0; r < size; r++) {
				clear(grid[r][col], r);
			}
		} break;
		case Unstable:
		{
			view->setLabel("Unstable");
			int sz = size; // looks pretty

			int rMin = (row - r >= 0)? row - r : 0;
			int rMax = (row + r < sz)? row + r : sz - 1;
			int cMin = (col - r >= 0)? col - r : 0;
			int cMax = (col + r < sz)? col + r : sz - 1;

			for (int r = rMin; r <= rMax; r++) {
				for (int c = cMin; c <= cMax; c++) {
					clear(grid[r][c], r);
				}
			}	   
		} break;
		case Psychedelic:
		{
			view->setLabel("Psychedelic");
			for (int r = 0; r < size; r++) {
				for (int c = 0; c < size; c++) {
					if (grid[r][c].getColour() == tColour) {
						clear(grid[r][c], r);
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

	PRNG rand;
	
	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			int randRow = rand(0, size-1);
			int randCol = rand(0, size-1);

			grid[r][c].swap(grid[randRow][randCol]);

			if (grid[r][c].isReady() ||
				grid[randRow][randCol].isReady()) {

				grid[r][c].swap(grid[randRow][randCol]);

			} else {

				view->setLocked(r, c, grid[r][c].isLocked());
				view->setColour(r, c, grid[r][c].getColour());
				view->setType(r, c, grid[r][c].getType());

				view->setLocked(randRow, randCol, grid[randRow][randCol].isLocked());
				view->setColour(randRow, randCol, grid[randRow][randCol].getColour());
				view->setType(randRow, randCol, grid[randRow][randCol].getType());
			}

			grid[r][c].clearReady();
			grid[r][c].clearNotified();

			grid[randRow][randCol].clearReady();
			grid[randRow][randCol].clearNotified();
		}
	}

	// recheck (necessary)
	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			grid[r][c].notify();
			
			if (grid[r][c].isReady()) scramble();
		}
	}

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			grid[r][c].setNotified(false);
		}
	}
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
