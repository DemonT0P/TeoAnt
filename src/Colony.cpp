#include "Colony.h"

#include "flags.h"

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>

#include "Debug.h"
#include "Utils.h"

namespace cfpmm {

Colony::Colony(const Instance* _instance, std::size_t _nAnts,
		float _evaporationRatio, double _alpha, double _beta, std::size_t _mbi) :
		instance(_instance), nAnts(_nAnts), maxIterations(_mbi), ants(
				_nAnts, nullptr), evaporationRatio(_evaporationRatio), alpha(
				_alpha), beta(_beta) {

	if (evaporationRatio >= 1 || evaporationRatio <= 0) {
		std::cerr
				<< "Evaporation ratio must be greater then 0 and less then 1\n";
		exit(-1);
	}

	assert(instance != nullptr);

	initialize();
}

Colony::~Colony() {

	for (auto& ant : ants) {
		delete ant;
	}
}

void Colony::initialize() {

	constexpr double initialPheromoneValue = 1;

	pheromoneList = std::vector<std::vector<double> >(instance->nKnapsacks,
			std::vector<double>(instance->nItems, initialPheromoneValue));

	for (int i = 0; i < nAnts; ++i) {
		ants[i] = new Ant(instance, pheromoneList, alpha, beta);
	}
}

void Colony::evaporate() {
	for (auto& l : pheromoneList) {
		for (auto& f : l) {
			f *= evaporationRatio;
		}
	}
}

std::tuple<Solution, int> Colony::run() {

	std::vector<int> solutionValues;
	Solution bestSolution(instance);

	int iterationsAux = 0;
	int iterations = 0;

	while (iterationsAux < maxIterations) {

		iterateOverAnts();

		solutionValues = getSolutionValues();

		bool updated = false;

		for (int i = 0; i < this->nAnts; ++i) {
			if (solutionValues.at(i) > bestSolution.getValue()) {
				bestSolution = this->ants.at(i)->getSolution();

				updated = true;
			}
		}

		if (!updated) {
			++iterationsAux;
		} else {
			iterationsAux = 0;
		}

		++iterations;
	}

	return std::make_tuple(bestSolution, iterations);
}

void Colony::iterateOverAnts() {

	for (int a = 0; a < nAnts; ++a) {
		ants[a]->findSolution();
	}

	evaporate();
	reinforce();
}

std::vector<int> Colony::getSolutionValues() {
	std::vector<int> solutionValues;

	for (int i = 0; i < this->nAnts; ++i) {
		solutionValues.push_back(this->ants.at(i)->getSolution().getValue());
	}

	return solutionValues;
}

void Colony::reinforce() {
	Ant* bestAnt = ants[0];

	for (int i = 1; i < ants.size(); ++i) {
		auto& ant = ants[i];

		if (ant->getValue() > bestAnt->getValue()) {
			bestAnt = ant;
		}
	}

	for (int i = 0; i < instance->nKnapsacks; ++i) {
		for (int j = 0; j < instance->nItems; ++j) {
			if (bestAnt->getSolution().isSelected(j, i)) {
				pheromoneList[i][j] += (1 - evaporationRatio);
			}
		}
	}
}

}
