#include "PuzzlePiece.h"
#include "PuzzleIndividual.h"
#include <vector>
#include <thread>

PuzzleIndividual::PuzzleIndividual() {

}

PuzzleIndividual::PuzzleIndividual(vector<PuzzlePiece> pieces, int frameWidth, int frameHeight, int initialMutationSize, int MAX_MUTATION, double learningRate, double fillFitnessPercentage, double valueFitnessPercentage, double boxInFitnessPercentage) {
	this->puzzle = pieces;
	this->frameWidth = frameWidth;
	this->frameHeight = frameHeight;
	this->frameLength = frameLength;

	this->fitness = 0.0;

	this->mutationSize = initialMutationSize;

	this->MAX_MUTATION = MAX_MUTATION;

	this->learningRate = learningRate;

	this->fillFitnessPercentage = fillFitnessPercentage;
	this->valueFitnessPercentage = valueFitnessPercentage;
	this->boxInFitnessPercentage = boxInFitnessPercentage;

}

vector<PuzzlePiece> PuzzleIndividual::getPuzzle() {
	return this->puzzle;
}

double PuzzleIndividual::getFitness() {
	return this->fitness;
}

void PuzzleIndividual::setFitness(double fitness) {
	this->fitness = fitness;
}

double PuzzleIndividual::fitnessEval() {

	vector<vector<vector<int>>> puzzleFrame = vector<vector<vector<int>>>(frameLength, vector<vector<int>>(frameWidth, vector<int>(frameHeight, 0)));

	int maxX = 0;
	int maxY = 0;
	int maxZ = 0;

	int minX = 1000;
	int minY = 1000;
	int minZ = 1000;

	int availableValue = 0;

	for (int p = 0; p < puzzle.size(); p++) { //Trimming the summing tripple for loop

		availableValue += puzzle[p].getValue();

		if (maxX != frameLength) {
			if (maxX <= puzzle[p].getX() && puzzle[p].getX() < frameLength) {
				maxX = puzzle[p].getX();
			}
			else if(puzzle[p].getX() >= frameLength) {
				maxX = frameLength;
			}
		}

		if (maxY != frameWidth) {
			if (maxY <= puzzle[p].getY() && puzzle[p].getY() < frameWidth) {
				maxY = puzzle[p].getY();
			}
			else if (puzzle[p].getY() >= frameLength) {
				maxY = frameLength;
			}
		}

		if (maxZ != frameHeight) {
			if (maxZ <= puzzle[p].getZ() && puzzle[p].getZ() < frameHeight) {
				maxZ = puzzle[p].getZ();
			}
			else if (puzzle[p].getZ() >= frameHeight) {
				maxZ = frameHeight;
			}
		}

		if (minX != 0) {
			if (minX >= puzzle[p].getX() && puzzle[p].getX() > 0) {
				minX = puzzle[p].getX();
			}
			else if (puzzle[p].getX() == 0) {
				minX = 0;
			}
		}
		
		if (minY != 0) {
			if (minY >= puzzle[p].getY() && puzzle[p].getY() > 0) {
				minY = puzzle[p].getY();
			}
			else if (puzzle[p].getY() == 0) {
				minY = 0;
			}
		}

		if (minZ != 0) {
			if (minZ >= puzzle[p].getZ() && puzzle[p].getZ() > 0) {
				minZ = puzzle[p].getZ();
			}
			else if (puzzle[p].getZ() == 0) {
				minZ = 0;
			}
		}

	}



	for (int p = 0; p < puzzle.size(); p++) {


		for (int i = puzzle[p].getY(); i < puzzle[p].getY() + puzzle[p].getHeight(); i++) {
			for (int j = puzzle[p].getX(); j < puzzle[p].getX() + puzzle[p].getWidth(); j++) {

				if (i < puzzleFrame.size() && j < puzzleFrame[i].size()) {

					//cout << "PUZZLE PIECES IN PUZZLE FRAME" << endl;
					//cout << i << " < " << puzzleFrame.size() << endl;
					//cout << j << " < " << puzzleFrame[i].size() << endl;

					puzzleFrame[i][j] = 0;
				}
				else {
					continue;
				}

			}
		}

	}

	//cout << "PUZZLE FRAME MADE" << endl; 

	int occupiedSpace = 0;

	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			for (int k = minZ; k < maxZ; k++) {
				occupiedSpace += puzzleFrame[i][j][k];
			}
		}
	}

	double occupiedPercent = ((double)occupiedSpace / (double)(frameWidth*frameHeight*frameLength)) * 100.0;
	double


	return ;
}


void PuzzleIndividual::mutate(double mutationRate) {

	for (int i = 0; i < puzzle.size(); i++) {

		double mutation = dRand(0, 1);

		if (mutation < mutationRate) { //Decide whether you will mutate the piece of not

			int choice = rand() % 4; //4 mutations possible

			//1. Move with respect to X
			//2. Move with respect to Y
			//3. Rotate
			//4. Randomize location

			int change = 0;
			int changeCounter = 0;

			switch (choice) {
			case 0:
			case 1:
				//Changing X or Y by an amount

				//Selecting magnetude of mutation size
				this->mutationSize = mutationSize * exp(this->learningRate * dRand(0, 1));

				//Selecting sign of mutation size
				this->mutationSize = rand() % 2 == 0 ? this->mutationSize : -1 * this->mutationSize;

				//Bounding mutation size
				if (mutationSize > MAX_MUTATION) {
					this->mutationSize = MAX_MUTATION;
				}

				if (mutationSize < -1 * MAX_MUTATION) {
					this->mutationSize = -1 * MAX_MUTATION;
				}

				change = round(this->mutationSize * dRand(0, 1));

				//Applying mutation size
				if (choice == 0) {
					while (puzzle[i].getX() + change > frameWidth || puzzle[i].getX() + change < 0) {
						change = round(change / 2);
						changeCounter++;

						if (changeCounter > 5) {
							change = 0;
							break;
						}

					}

					this->puzzle[i].moveX(change);
				}
				else {
					while (puzzle[i].getY() + change > frameWidth || puzzle[i].getY() + change < 0) {
						change = round(change / 2);
						changeCounter++;
						if (changeCounter > 5) {
							change = 0;
							break;
						}
					}

					this->puzzle[i].moveY(change);
				}

				break;

			case 2:
				//Rotating the piece
				this->puzzle[i].rotatePiece();
				break;
			case 3:
				//Randomly resetting the piece's location
				this->puzzle[i].setX(rand() % frameWidth);
				this->puzzle[i].setY(rand() % frameHeight);
				break;
			}

		}

	}

}

void PuzzleIndividual::crossover(PuzzleIndividual & partner, double crossoverRate) {

	PuzzleIndividual child = *this;

	for (int i = 0; i < this->getPuzzle().size(); i++) {

		//Roll the crossover amount
		double crossover = dRand(0, 1);

		if (crossover < crossoverRate) {

			int choice = rand() % 3;//3 crossover possible

			//1. Only crossover x
			//2. Only crossover y
			//3. crossover x and y

			int newY = 0;
			int newX = 0;

			switch (choice) {
			case 0:

				newX = round((getPuzzle()[i].getX() + partner.getPuzzle()[i].getX()) / 2);

				child.puzzle[i].setX(newX);

				break;
			case 1:

				newY = round((this->getPuzzle()[i].getY() + partner.getPuzzle()[i].getY()) / 2);

				child.puzzle[i].setY(newY);

				break;
			case 2:

				newX = round((this->getPuzzle()[i].getX() + partner.getPuzzle()[i].getX()) / 2);
				newY = round((this->getPuzzle()[i].getY() + partner.getPuzzle()[i].getY()) / 2);

				child.puzzle[i].setX(newX);
				child.puzzle[i].setY(newY);

				break;
			}
		}
	}

	child.setFitness(child.fitnessEval());

	PuzzleIndividual temp[] = { child, *this, partner };

	//SORT NEW CHILD WITH PARENTS
	if (temp[1].getFitness() < temp[0].getFitness()) {
		swap(temp[0], temp[1]);
	}

	if (temp[2].getFitness() < temp[1].getFitness())
	{
		swap(temp[1], temp[2]);
		if (temp[1].getFitness() < temp[0].getFitness())
			swap(temp[1], temp[0]);
	}

	//IF CHILD CAME OUT BETTER THAN EITHER PARENT
	if (temp[2].getFitness() == child.getFitness() || temp[1].getFitness() == child.getFitness()) {
		this->puzzle = temp[2].puzzle;
		partner.puzzle = temp[1].puzzle;

	}

}

double PuzzleIndividual::dRand(double min, double max) {
	double f = (double)rand() / RAND_MAX;
	return min + f * (max - min);
}

string PuzzleIndividual::toString() {
	string result = "";
	for (int i = 0; i < puzzle.size(); i++) {
		result += puzzle[i].toString() + "; ";
	}
	return result;
}
