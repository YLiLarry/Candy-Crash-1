#include "generator.h"
#include <random>
using namespace std;

string Generator::randomSquare(int level) {

	if (level == 1) {

		// colour probabilities
		double pWhite = ((double)1) / 3;
		double pRed   = ((double)1) / 3;
		double pGreen = ((double)1) / 6;
		double pBlue  = ((double)1) / 6;

		discrete_distribution<int> colourDist { pWhite, pRed, pGreen, pBlue };

		string randomSquare = "";

		// until we change name
		char advanced_feature = '_';
	}
}
