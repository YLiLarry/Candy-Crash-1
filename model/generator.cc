#include "../public/global.h"
#include "generator.h"
#include <iostream>
#include <random>
#include <ctime>
using namespace std;

int Generator::produced = 0;

string Generator::randomSquare(int level) {

	produced++;
	string randomSquare = "";
	default_random_engine gen(produced);

	if (level == 1) {
		
		// type distribution (uniform probability for specials);
		discrete_distribution<int> typeDist { 0, 1, 1, 1, 1 };

		// colour probabilities
		double pWhite = ((double)1) / 3;
		double pRed   = ((double)1) / 3;
		double pGreen = ((double)1) / 6;
		double pBlue  = ((double)1) / 6;

		// colour distribution
		discrete_distribution<int> colourDist { pWhite, pRed, pGreen, pBlue };


		char extra = '_';
		int type = (produced % 5 == 0)? typeDist(gen) : 0;
	 	int colour = colourDist(gen);

		randomSquare += extra;

		switch (type) {

			case 0: randomSquare += '_'; break;
			case 1: randomSquare += 'h'; break;
			case 2: randomSquare += 'v'; break;
			case 3: randomSquare += 'b'; break;
			case 4: randomSquare += 'p'; break;
		}

		switch (colour) {

			case 0: randomSquare += '0'; break;
			case 1: randomSquare += '1'; break;
			case 2: randomSquare += '2'; break;
			case 3: randomSquare += '3'; break;
		}
	}

	if (level == 2) {

		cerr << produced << endl;
		discrete_distribution<int> colourDist { 1, 1, 1, 1 };

		char extra = (produced > 80 && produced <= 100)? 'l' : '_';

		randomSquare += extra;

		char type = '_';

		randomSquare += type;

		int colour = colourDist(gen);

		switch (colour) {

			case 0: randomSquare += '0'; break;
			case 1: randomSquare += '1'; break;
			case 2: randomSquare += '2'; break;
			case 3: randomSquare += '3'; break;
		}
	}

	return randomSquare;
}
