#pragma once
#include <iostream>
#include <vector>
#include "PuzzlePiece.h"
#include <string>

using namespace std;

class PuzzleIndividual {

private:

	int frameLength;
	int frameHeight;
	int frameWidth;

	int mutationSize;
	int MAX_MUTATION;

	double learningRate;

	double fillFitnessPercentage;
	double valueFitnessPercentage;
	double boxInFitnessPercentage;

public:

	vector<PuzzlePiece> puzzle;
	double fitness;

	PuzzleIndividual();
	PuzzleIndividual(vector<PuzzlePiece> pieces, int frameWidth, int frameHeight, int initialMutationSize, int MAX_MUTATION, double learningRate, double fillFitnessPercentage, double valueFitnessPercentage, double boxInFitnessPercentage);

	vector<PuzzlePiece> getPuzzle();
	double getFitness();
	void setFitness(double fitness);

	double fitnessEval();

	void mutate(double mutationRate);

	void crossover(PuzzleIndividual & partner, double crossoverRate);

	double dRand(double min, double max);

	string toString();

};
