#pragma once
#include <iostream>
#include <vector>
#include "PuzzlePiece.h"
#include <string>

using namespace std;

class PuzzleIndividual {

private:

	int frameHeight;
	int frameWidth;

	double overallLearningRate;
	double coordinateWiseLearningRate;
	double mutationSize;
	int MAX_MUTATION;

	double C;
	double CHANGE_SUCCESS_RATE;

	int mutationSuccesses;
	int mutationCount;

	int crossoverSuccesses;
	int crossoverCount;

public:

	double mutationRate;
	double crossoverRate;

	vector<PuzzlePiece> puzzle;
	double fitness;

	PuzzleIndividual();
	PuzzleIndividual(vector<PuzzlePiece> pieces, int n, double C, int MAX_MUTATION, double CHANGE_SUCCESS_RATE, int frameWidth, int frameHeight);

	vector<PuzzlePiece> getPuzzle();
	double getFitness();
	void setFitness(double fitness);

	double fitnessEval();

	void mutate();

	void crossover(PuzzleIndividual & partner);

	double dRand(double min, double max);

	string toString();

};
