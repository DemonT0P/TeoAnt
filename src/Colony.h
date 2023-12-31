#ifndef COLONY_H_
#define COLONY_H_

#include <vector>
#include <tuple>

#include "Ant.h"
#include "Instance.h"

namespace cfpmm {

/**
 * Class that represent colony of ants.
 */
class Colony {
public:
	/**
	 * @param instance Instance of problem
	 * @param nAnts Number of ants
	 * @param evaporationRatio Evaporation ratio
	 * @param alpha Value of alpha
	 * @param beta Value of beta
	 * @param mbi Number of iterations without improvement
	 */
	Colony(const Instance* instance, std::size_t nAnts, float evaporationRatio,
			double alpha, double beta, std::size_t mbi = 50);
	virtual ~Colony();

	/**
	 * @return Matrix of pheromones.
	 */
	const std::vector<std::vector<double>>& getPheromoneList() const {
		return pheromoneList;
	}

	/**
	 * @return List of Ants.
	 */
	const std::vector<Ant*>& getAnts() const {
		return ants;
	}

	/**
	 * Run algorithm.
	 *
	 * @return Tuple containing final Solution and number of iterations spent.
	 */
	std::tuple<Solution, int> run();

private:

	std::vector<int> getSolutionValues();

	/**
	 * 	Initialize the ants and pheromone list.
	 */
	void initialize();

	/**
	 * Iteration method.
	 */
	void iterateOverAnts();

	/**
	 * Apply evaporation of pheromones.
	 */
	void evaporate();

	/**
	 * Apply reinforce of pheromones.
	 */
	void reinforce();

	/**
	 * Number of ants.
	 */
	std::size_t nAnts;

	/**
	 * Number of iterations without improvement.
	 */
	std::size_t maxIterations;

	/**
	 * Instance of problem.
	 */
	const Instance* instance;

	/**
	 * List of ants.
	 */
	std::vector<Ant*> ants;

	/**
	 * Evaporation ratio.
	 */
	float evaporationRatio;

	/**
	 * Alpha value.
	 *
	 * This value set the influence of pheromone on choice of an item.
	 */
	double alpha;

	/**
	 * Alpha value.
	 *
	 * This value set the influence of heuristic value on choice of an item.
	 */
	double beta;

	/**
	 * Matrix of pheromones.
	 */
	std::vector<std::vector<double>> pheromoneList;
};

}

#endif /* COLONY_H_ */
