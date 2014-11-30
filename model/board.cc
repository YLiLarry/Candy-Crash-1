#include <fstream>
#include <sstream>
#include <cmath>
#include "board.h"
#include "generator.h"
#include "../view/textview/textview.h"
#include "../public/global.h"
#include "PRNG.h"

using namespace std;

// change

//
// Constructing a new board of size n.
//
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

	// Ready to play
	loadLevel(level);
}

//
// Board destructor.
//
Board::~Board() {

	for (int i = 0; i < size; i++) {

		delete[] grid[i];
	}

	delete[] grid;

	delete generate;
	delete view;
}

//
// Sets up the board as required for a certain level.
// Level 1 is read from sequene.txt and for rest of the
// levels, it sets up the grid by acquiring squares from
// the Generator.
//
void Board::loadLevel(int level) {

	if (level == 0) {

		ifstream file("sequence.txt");

		if (! file.good()) {
			throw string("unable to read the sequence.txt file");
		}

		string square;

		for (int r = 0; r < size; r++) {
			for (int c = 0; c < size; c++) {

				file >> square;

				setNewSquare(grid[r][c], square);
				grid[r][c].setNeighbours();

				if (r == size - 1 && c == size - 1) {

					file >> levelZeroColours;
				}
			}
		}
		
		view->setScore(score);
		view->setLevel(level);

	} else {
		
		// Reset the number of produced squares for this level.
		generate->produced = 0;

		for (int r = 0; r < size; r++) {
			for (int c = 0; c < size; c++) {

				string square = generate->randomSquare(level);
				setNewSquare(grid[r][c], square);
				grid[r][c].setNeighbours();
			}
		}

		// This is necessary as the squares are not fully
		// random. e.g. "every 5th square is a special square"
		scramble(true);
		
		view->setLevel(level);
		view->setScore(score);
		view->draw();
	}

	view->draw();
}

//
// Checks the conditions necessary to level up
//
void Board::checkLevel() {

	if (score >= initScore + 200 && level == 0) {
		
		level = 1;

	} else if (score >= initScore + 300 && level == 1) {

		level = 2;

	} else if (score >= initScore + 500 && level == 2 && unlocked == 20) {

		cout << "WINNER!" << endl;
		return;
	}

	initScore = score;
}

//
// Swaps a square with with its neighbour in Direction d.
//
void Board::swap(int row, int col, Direction d) {

	// resets the score tracking variables
	cleared = 0;
	turnScore = 0;
	chain = 0;
	chainMode = false;
	emptyBoard = false;

	// swaps the square with its neighbour
	grid[row][col].swapWith(d);
	
	// draw the board to see the swap take place
	view->draw();

	// start clearing matched squares originating
	// from neighbour
	clearAt(*grid[row][col].neighbour[d]);

	// start clearing matched squares at the original
	// coordinates
	clearAt(grid[row][col]);

	// if nothing was cleared, swap back
	if (!cleared) grid[row][col].swapWith(d);

	// clear notification status from squares
	// required for matching to work properly next turn
	unNotifyAll();

	// this loop is where the dropping and chainreaction
	// takes place, loop ends when chainMode ends,
	// which means after dropping squares, there aren't
	// anymore matches, OR when the board is fully cleared.
	do {

		dropSquares();
		view->draw();
		chainReaction();
		view->draw();

	} while (chainMode && !emptyBoard);

	// adding to overall score
	score += turnScore;

	view->setScore(score);
	view->draw();

	ostringstream ss;
	ss << "cleared:  " << cleared << endl;
	if (level >= 2) ss << "unlocked: " << unlocked << endl;
	ss << "chains :  " << chain << endl;
	ss << "scored : +" << turnScore << endl;
	view->print(ss.str());

	checkLevel();
}

//
// Collect all the matched squares and store pointers to them
// in hMatch or vMatch vectors depending on the orientation.
// The orientation is relative the referenced root square.
//
void Board::collectMatched(Square &root) {

	// Clear vectors before collecting.
	hMatch.clear();
	vMatch.clear();

	// for easier referencing.
	int row = root.getRow();
	int col = root.getCol();

	// Collect all matched square in horizontal direction.
	for (int r = 0; r < size; r++) {
		if (grid[r][col].isReady()) {

			vMatch.push_back(&grid[r][col]);
		}
	}

	// Collect all matched squares in the vertical direction.
	for (int c = 0; c < size; c++) {
		if (grid[row][c].isReady()) {

			hMatch.push_back(&grid[row][c]);
		}
	}
}

//
// Starts clearing matched squares from a given
// row and column of the square reference.
//
void Board::clearAt(Square &root) {

	// Push the matched squares on hMatch and vMatch vectors.
	collectMatched(root);

	// Backup the colour, required for settings special squares.
	Colour backup = root.getColour();

	// The radius of the bomb. Initialized to 0.
	int radius = 0;

	// Sets the appropriate radius of the bomb.
	if (hMatch.size() > 3 || vMatch.size() > 3) {

		radius = 2;

	} else if (hMatch.size() == 3 || vMatch.size() == 3) {

		radius = 1;
	}

	// This if-block determines, what kind of match took place.
	if (hMatch.size() < 3 && vMatch.size() < 3) {

		// No match.
		return;

	} else if (hMatch.size() == 3 && vMatch.size() == 3) {

		// An L match, OR a T match.
		//
		// In this version both types of matches
		// will award the player with an Unstable square.

		for (int i = 0; i < 3; i++) { clear(*hMatch[i], radius); }
		for (int i = 0; i < 3; i++) { clear(*vMatch[i], radius); }

		root.setColour(backup);
		root.setType(Unstable);

		view->setColour(root.getRow(), root.getCol(), backup);
		view->setType(root.getRow(), root.getCol(), Unstable);

	} else if (hMatch.size() > vMatch.size()) {

		// A horizontal match.
		//
		// A Lateral or a Psychedelic square is earned
		// depending on the size of the match.

		int n = (int)hMatch.size();

		for (int i = 0; i < n; i++) { clear(*hMatch[i], radius); }

		if (n == 4) {
			
			root.setColour(backup);
			root.setType(Lateral);
			view->setColour(root.getRow(), root.getCol(), backup);
			view->setType(root.getRow(), root.getCol(), Lateral);

		} else if (n == 5) {

			root.setColour(backup);
			root.setType(Psychedelic);
			view->setColour(root.getRow(), root.getCol(), backup);
			view->setType(root.getRow(), root.getCol(), Psychedelic);
		}

	} else if (hMatch.size() < vMatch.size()) {

		// A vertical match.
		//
		// An Upright or a Psychedelic square is earned
		// depending on the size of the match.

		int n = (int)vMatch.size();

		for (int i = 0; i < n; i++) { clear(*vMatch[i], radius); }

		if (n == 4) {
			
			root.setColour(backup);
			root.setType(Upright);
			view->setColour(root.getRow(), root.getCol(), backup);
			view->setType(root.getRow(), root.getCol(), Upright);

		} else if (n == 5) {

			root.setColour(backup);
			root.setType(Psychedelic);
			view->setColour(root.getRow(), root.getCol(), backup);
			view->setType(root.getRow(), root.getCol(), Psychedelic);
		}
	}
}

//
// This method clears a referenced square. It is also provided
// the radius which should be cleared if an Unstable square is
// found. It overrides the radius if 3+ squares have been cleared.
//
void Board::clear(Square &sq, int r) {

	// Ignore empty squares.
	if (sq.getColour() == Empty)  return;

	// Unlock a locked square.
	if (sq.isLocked()) {

		sq.setLocked(false);
		sq.setReady(false);
		view->setLocked(sq.getRow(), sq.getCol(), false);

		unlocked++;
		
		return;
	}

	// Backing up the Colour and Type of te square,
	// that is being cleared.
	Colour tColour = sq.getColour();
	Type tType = sq.getType();

	// Clear an unlocked square
	sq.setColour(Empty);
	sq.setType(Basic);
	sq.setReady(false);

	cleared++;

	// Override the given.
	if (cleared > 3) r = 2;

	// Updates the turn score.
	switch (cleared) {

		case 0: case 1: break;
		case 2:
		case 3: turnScore = 3; break;
		case 4: turnScore = 8; break;
		case 5: turnScore = 15; break;
		default: turnScore = 4 * cleared;
	}

	// If chainMode is true, player earns bonus points.
	if (chainMode) turnScore *= pow(2, chain);
	
	// For easier referencing.
	int row = sq.getRow();
	int col = sq.getCol();

	// Clears the square on the view side.
	view->destroy(row, col);
	view->setScore(score);

	// Show the cleared square.
	view->draw();

	// This switch block handles clearing special squares.
	switch (tType) {

		case Basic: break;
		case Lateral: 
		{
			// Clears the entire row.
			for (int c = 0; c < size; c++) {
				clear(grid[row][c], r);
			}					  
		} break;			  
		case Upright:
		{
			// Clears the entire column.
			for (int r = 0; r < size; r++) {
				clear(grid[r][col], r);
			}
		} break;
		case Unstable:
		{
			// Determines the min/max rows and columns to
			// clearing, using the radius.
			int rMin = (row - r >= 0)? row - r : 0;
			int rMax = (row + r < size)? row + r : size - 1;
			int cMin = (col - r >= 0)? col - r : 0;
			int cMax = (col + r < size)? col + r : size - 1;

			// Creates either a 3x3 or 5x5 hole on the grid.
			for (int r = rMin; r <= rMax; r++) {
				for (int c = cMin; c <= cMax; c++) {
					clear(grid[r][c], r);
				}
			}	   
		} break;
		case Psychedelic:
		{
			// Clears all squares with colour: tColour.
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

//
// When a square is empty at the top row, and requires
// new colour and type information, this method is called
// to set the square with new colour and type.
//
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

		setNewSquare(sq, generate->randomSquare(level));
	}
}

//
// Parses strSquare and sets the referenced square
// as specified by the string representation.
//
void Board::setNewSquare(Square &sq, string strSquare) {

	// The locked status of the square
	bool locked = (strSquare[0] == 'l');

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
		default: 
		{throw string("unexpected square type: '") + strSquare[1] + "'";}
	}

	// Setting the actual scquare
	sq.setLocked(locked);
	sq.setColour(colour);
	sq.setType(type);

	int r = sq.getRow();
	int c = sq.getCol();

	// Updating the view
	view->setLocked(r, c, locked);
	view->setColour(r, c, colour);
	view->setType(r, c, type);
}

//
// Drops squares that are floating.
//
void Board::dropSquares() {

	for (int c = 0; c < size; c++) {
		grid[0][c].drop();

		// Supply the top row with new squares.
		while (grid[0][c].getColour() == Empty) {

			setNewSquare(grid[0][c]);
			grid[0][c].drop();
		}
	}
}

//
// Checks for matches across the whole grid.
// All matched squares are cleared, and if no
// matched squares are detected, or the board
// consists of only empty squares chainMode ends.
//
void Board::chainReaction() {

	// Assume there will be no matched squares.
	chainMode = false;

	notifyAll();

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			if (grid[r][c].isReady()) {

				// Increment number of chains
				if (!chainMode) chain++;

				chainMode = true;
				clear(grid[r][c], 4);
			}
		}
	}

	// Assume board is empty.
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

//
// This method checks for a valid move within the current
// state of the grid. Returns the first valid move in the
// form a of a string: [row] [col] [up|down|left|right]
//
string Board::validMove() {

	ostringstream ss;

	// Assume no match is found.
	bool foundMatch = false;

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			// For swapping with neighbours in each direction.
			for (int d = 0; d < 4; d++) {

				// Invisibly swap squares with it's neighbours
				// If the neighbour exists (not NULL).
				if (grid[r][c].neighbour[d]) {

					grid[r][c].swap((Direction)d);

					// A match is found
					if (grid[r][c].isReady() ||
						grid[r][c].neighbour[d]->isReady()) {

						ss << "swap " << r << " " << c << " "
						   << dir2str((Direction)d);

						foundMatch = true;
					}

					// Clear matched status and notifications.
					grid[r][c].clearReady();
					grid[r][c].clearNotified();

					// Swap back.
					grid[r][c].swap((Direction)d);

					// Clear matched status and notifications.
					grid[r][c].clearReady();
					grid[r][c].clearNotified();

					// Return the first move found
					if (foundMatch) return ss.str();
				}
			}
		}
	}

	// Default hint if no moves are found.
	ss << "no moves left, scramble?";

	return ss.str();
}

//
// Prints a valid move or suggests the player to scramble
// if no moves are left.
//
void Board::hint() {

	view->print("hint: " + validMove());
}

//
// Scrambles the board randomly, while making sure
// no matched squares are made while scrambling.
//
void Board::scramble(bool force) {

	if (!force && validMove() != "no moves left, scramble?") {

		view->print("there are still moves left, need a hint?");
		return;
	}

	PRNG rand;
	
	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			// Getting a random row and column.
			int rndR = rand(0, size-1);
			int rndC = rand(0, size-1);

			// Swapping with the randomly selected square.
			// This is done invisibly.
			grid[r][c].swap(grid[rndR][rndC]);

			// Checking for matched squares.
			if (grid[r][c].isReady() ||
				grid[rndR][rndC].isReady()) {

				// If matched squarse are found, swap back.
				grid[r][c].swap(grid[rndR][rndC]);

			} else {

				// If no matches occur, update the view.
				view->setLocked(r, c, grid[r][c].isLocked());
				view->setColour(r, c, grid[r][c].getColour());
				view->setType(r, c, grid[r][c].getType());

				// Updating the view at the randomly selected coordinate.
				view->setLocked(rndR, rndC, grid[rndR][rndC].isLocked());
				view->setColour(rndR, rndC, grid[rndR][rndC].getColour());
				view->setType(rndR, rndC, grid[rndR][rndC].getType());
			}

			// Clears matched status and notifications

			grid[r][c].clearReady();
			grid[r][c].clearNotified();

			grid[rndR][rndC].clearReady();
			grid[rndR][rndC].clearNotified();
		}
	}

	// recheck (necessary)
	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			grid[r][c].notify();
			
			// Rescramble if matches are found.
			if (grid[r][c].isReady()) scramble(true);
		}
	}

	unNotifyAll();
}
//
// Notifies all squares to check for matches.
// 
void Board::notifyAll() {

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			grid[r][c].notify();
		}
	}
}

//
// Clears notification status for all squares.
//
void Board::unNotifyAll() {

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {

			grid[r][c].setNotified(false);
		}
	}
}



// debuggin purposes
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
