#include "json/json.h"
#include <iostream>
#include "PuzzleIndividual.h"
#include "PuzzlePiece.h"
#include <fstream>

using namespace std;

class PuzzleProblem {

private:

	int maxGenerations = 500;
	int populationSize = 500;

	int survivalThreashold = 5;

	double fillFitnessPercentage = 0.6;
	double valueFitnessPercentage = 0.3;
	double boxInFitnessPercentage = 0.1;

	int initialMutationSize = 50;
	int MAX_MUTATION = 200;

	double goalDifference = 0.4;
	double pmChangeRate = 0.05;

	double pm = 0.5;
	double pc = 0.7;

	double learningRate = 1 / sqrt(2 * populationSize);

	vector<PuzzleIndividual> pop;

public:

	PuzzleProblem(vector<vector<PuzzlePiece>> newPuzzles, int frameLength, int frameWidth, int frameHeight) {

		for (int i = 0; i < newPuzzles.size(); i++) {
			pop.push_back(PuzzleIndividual(newPuzzles[i], frameLength, frameWidth, frameHeight, initialMutationSize, MAX_MUTATION, learningRate, fillFitnessPercentage, valueFitnessPercentage, boxInFitnessPercentage));
		}

	}

	void startAlgorithm() {

		for (int g = 0; g < maxGenerations; g++) {

			cout << "Generation " << g << endl;

			//Mutation
			for (int i = 0; i < populationSize - survivalThreashold; i++) {
				pop[i].mutate(pm);
			}
			
			//Crossover
			vector<int> availableCrossoverItems;

			for (int i = 0; i < populationSize; i++) {
				availableCrossoverItems.push_back(i);
			}

			for (int i = 0; i < populationSize / 2; i++) {

				int firstIndex = availableCrossoverItems.size() - 1;
				int first = availableCrossoverItems[firstIndex];

				availableCrossoverItems.pop_back();

				int secondIndex = rand() % availableCrossoverItems.size();
				int second = availableCrossoverItems[secondIndex];

				availableCrossoverItems.erase(availableCrossoverItems.begin() + secondIndex);

				PuzzleIndividual & partner = pop[second];

				pop[first].crossover(partner, pc);

			}

			//Fitness
			for (int i = 0; i < populationSize; i++) {
				pop[i].setFitness(pop[i].fitnessEval());
			}
		
			//Sorting
			sort(pop.begin(), pop.end(), [](PuzzleIndividual const & a, PuzzleIndividual const & b) -> bool {
				return a.fitness < b.fitness;
			});

			//Recalculating pm
			double totalFitness = 0;

			for (int i = 0; i < populationSize; i++) {
				totalFitness += pop[i].getFitness();
			}

			double averageFitness = totalFitness / populationSize;
			double fittestFitness = pop[pop.size() - 1].getFitness();

			double diff = fittestFitness - averageFitness;

			if (diff > goalDifference) {
				pm += pmChangeRate;
			}
			else {
				pm -= pmChangeRate;
			}

			//Print Status Info
			cout << "Fitness Leader (Fill%): " << pop[pop.size() - 1].fitness << endl;
			cout << "Pieces: " << pop[pop.size() - 1].toString() << endl;
			cout << "Mutation %: " << pm << endl;
			cout << "Crossover %: " << pc << endl << endl;

		}

	}

	static PuzzleProblem jsonToObj(string fileDir) {
		ifstream ifs(fileDir);

		string message = "";

		for (string line; getline(ifs, line);) {
			message += line;
		}

		//Json::Reader reader;
		Json::Value obj;
		//reader.parse(ifs, obj); // reader can also read strings

		Json::CharReaderBuilder builder;
		Json::CharReader *reader = builder.newCharReader();

		Json::Value output;
		string errors;

		reader->parse(message.c_str(), message.c_str() + message.length(), &obj, &errors);

		int pieces = 0;

		int length = obj["truckLength"].asInt();
		int width = obj["truckWidth"].asInt();
		int height = obj["truckHeight"].asInt();
		pieces = obj["numberOfBoxes"].asInt();
		vector<PuzzlePiece> puzzlePieces;

		for (const Json::Value& value : obj["boxArrangement"]["0"]) {
			puzzlePieces.push_back(PuzzlePiece(value[0].asInt(), value[1].asInt(), value[2].asInt(), value[3].asInt()));
		}

		ifs.close();

		return PuzzleProblem(length, width, height, pieces, puzzlePieces);

	}


};


int main() {
	
	//Call to Json Here to create PuzzleIndividual Object
	PuzzleProblem puzzleProblem = PuzzleProblem::jsonToObj("./TruckandPackages.json");

	//Start Algorithm
	puzzleProblem.startAlgorithm();

	cout << "Hello World" << endl;
	
	system("pause");

}