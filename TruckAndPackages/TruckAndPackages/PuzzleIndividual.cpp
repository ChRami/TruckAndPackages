#include "PuzzlePiece.h"
#include "PuzzleIndividual.h"
#include <vector>
#include <thread>

PuzzleIndividual::PuzzleIndividual() {

}

PuzzleIndividual::PuzzleIndividual(vector<PuzzlePiece> pieces, int frameLength, int frameWidth, int frameHeight, int initialMutationSize, int MAX_MUTATION, double learningRate, double fillFitnessPercentage, double valueFitnessPercentage, double boxInFitnessPercentage, int numberOfThreads) {
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

	this->numberOfThreads = numberOfThreads;

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
	int maxValue = 0;
	int availableBoxes = 0;

	for (int p = 0; p < puzzle.size(); p++) { //Trimming the summing tripple for loop

		maxValue += puzzle[p].getValue();

		if (puzzle[p].getX() + puzzle[p].getLength() < frameLength && puzzle[p].getY() + puzzle[p].getWidth() < frameWidth && puzzle[p].getZ() + puzzle[p].getHeight() < frameHeight) {
			availableValue += puzzle[p].getValue();
			availableBoxes++;
		}
		

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
			else if (puzzle[p].getY() >= frameWidth) {
				maxY = frameWidth;
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

	vector<thread> threads;

	for (int p = 0; p < puzzle.size(); p++) {

		PuzzlePiece currentPuzzle = puzzle[p];

		int split = puzzle[p].getLength()/numberOfThreads;

		int frameLength = this->frameLength;
		int frameWidth = this->frameWidth;
		int frameHeight = this->frameHeight;
		int numberOfThreads = this->numberOfThreads;

		for (int t = 0; t < numberOfThreads; t++) {
			threads.push_back(thread([t, p, currentPuzzle, frameLength, frameWidth, frameHeight, numberOfThreads, split, &puzzleFrame]() {

				int minX = currentPuzzle.getX() + (split*t);
				int maxX;

				if (t != numberOfThreads - 1) {
					maxX = currentPuzzle.getX() + (split*(t + 1));
				}
				else {
					maxX = currentPuzzle.getX() + currentPuzzle.getLength();
				}

				for (int i = minX; i < maxX; i++) {
					for (int j = currentPuzzle.getY(); j < currentPuzzle.getY() + currentPuzzle.getWidth(); j++) {
						for (int k = currentPuzzle.getZ(); k < currentPuzzle.getZ() + currentPuzzle.getWidth(); k++) {

							if (i < frameLength && j < frameWidth && k < frameHeight) {
								puzzleFrame[i][j][k] = 1;
							}
							else {
								continue;
							}

						}
					}
				}

			}));
		}	

		for (int i = 0; i < numberOfThreads; i++) {
			threads[i].join();
		}

		threads.clear();

	}

	int occupiedSpace = 0;

	vector<int> counts = vector<int>(numberOfThreads, 0);

	int split = (maxX - minX)/numberOfThreads;

	for (int t = 0; t < numberOfThreads; t++) {
		threads.push_back(thread([t, minX, maxX, minY, maxY, minZ, maxZ, split, puzzleFrame, &counts]() {

			int threadMin = minX + (split*t);
			int threadMax;

			if (t != counts.size() - 1) {
				threadMax = minX + (split*(t + 1));
			}
			else {
				threadMax = maxX;
			}

			for (int i = threadMin; i < threadMax; i++) {
				for (int j = minY; j < maxY; j++) {
					for (int k = minZ; k < maxZ; k++) {
						counts[t] += puzzleFrame[i][j][k];
					}
				}
			}

		}));
	}

	for (int i = 0; i < numberOfThreads; i++) {
		threads[i].join();
	}

	for (int i = 0; i < counts.size(); i++) {
		occupiedSpace += counts[i];
	}

	double occupiedPercent = ((double)occupiedSpace / (double)(frameWidth*frameHeight*frameLength)) * 100.0;
	double valuePercent = ((double)availableValue / (double)maxValue) * 100.0;
	double boxPercent = ((double)availableBoxes / (double) puzzle.size()) * 100.0;


	return occupiedPercent * fillFitnessPercentage + valuePercent * valueFitnessPercentage + boxPercent * boxInFitnessPercentage;
}


void PuzzleIndividual::mutate(double mutationRate) {

	for (int i = 0; i < puzzle.size(); i++) {

		double mutation = dRand(0, 1);

		if (mutation < mutationRate) { //Decide whether you will mutate the piece of not

			int choice = rand() % 3; //4 mutations possible

			//1. Move with respect to X, Y or Z
			//4. Rotate
			//5. Randomize location

			int change = 0;
			int changeCounter = 0;

			switch (choice) {
			case 0:
			{

				int choice = rand() % 3;

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
				else if (choice == 1) {
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

				else {
					while (puzzle[i].getZ() + change > frameWidth || puzzle[i].getZ() + change < 0) {
						change = round(change / 2);
						changeCounter++;

						if (changeCounter > 5) {
							change = 0;
							break;
						}

					}

					this->puzzle[i].moveZ(change);
				}

				break;
			}

			case 1:
			{
				//Rotating the piece
				int choice = rand() % 3;
				this->puzzle[i].rotatePiece(choice);
				break;
			}
			case 2:
			{
				//Randomly resetting the piece's location
				this->puzzle[i].setX(rand() % frameLength);
				this->puzzle[i].setY(rand() % frameWidth);
				this->puzzle[i].setZ(rand() % frameHeight);
				break;
			}
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

			int choice = rand() % 7;

			//1. Only crossover x
			//2. Only crossover y
			//3. Only crossover z
			//4. crossover x and y
			//5. crossover y and z
			//6. crossover x and z
			//7. crossover x, y and z

			int newY = 0;
			int newX = 0;
			int newZ = 0;

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
				newZ = round((this->getPuzzle()[i].getZ() + partner.getPuzzle()[i].getZ()) / 2);

				child.puzzle[i].setZ(newZ);

				break;
			case 3:

				newX = round((this->getPuzzle()[i].getX() + partner.getPuzzle()[i].getX()) / 2);
				newY = round((this->getPuzzle()[i].getY() + partner.getPuzzle()[i].getY()) / 2);

				child.puzzle[i].setX(newX);
				child.puzzle[i].setY(newY);

				break;
			case 4:
				newY = round((this->getPuzzle()[i].getY() + partner.getPuzzle()[i].getY()) / 2);
				newZ = round((this->getPuzzle()[i].getZ() + partner.getPuzzle()[i].getZ()) / 2);

				child.puzzle[i].setY(newY);
				child.puzzle[i].setZ(newZ);

				break;
			case 5:
				newX = round((this->getPuzzle()[i].getX() + partner.getPuzzle()[i].getX()) / 2);
				newZ = round((this->getPuzzle()[i].getZ() + partner.getPuzzle()[i].getZ()) / 2);

				child.puzzle[i].setX(newX);
				child.puzzle[i].setZ(newZ);

				break;
			case 6:
				newX = round((this->getPuzzle()[i].getX() + partner.getPuzzle()[i].getX()) / 2);
				newY = round((this->getPuzzle()[i].getY() + partner.getPuzzle()[i].getY()) / 2);
				newZ = round((this->getPuzzle()[i].getZ() + partner.getPuzzle()[i].getZ()) / 2);

				child.puzzle[i].setX(newX);
				child.puzzle[i].setY(newY);
				child.puzzle[i].setZ(newZ);

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
	if (temp[2].getFitness() == child.getFitness()) {
		this->puzzle = temp[2].puzzle;
		partner.puzzle = temp[1].puzzle;

		this->fitness = child.getFitness();
		partner.fitness = 0;
	}
	else if (temp[1].getFitness() == child.getFitness()) {
		this->puzzle = temp[2].puzzle;
		partner.puzzle = temp[1].puzzle;

		this->fitness = 0;
		partner.fitness = child.getFitness();

	}
	else {
		this->fitness = 0;
		partner.fitness = 0;
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
