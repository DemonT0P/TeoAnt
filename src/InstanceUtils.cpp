#include "InstanceUtils.h"

#include <cstdlib>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

namespace cfpmm {

constexpr auto MIN = 10;
constexpr auto MAX = 1000;

using namespace std;

InstanceUtils::~InstanceUtils() {

}

Instance* InstanceUtils::generate(int index, 
		std::size_t nItems, 
		std::size_t nKnapsacks,
		correlation correlated, 
		bool similar, 
		bool save) {

#define RAND(MIN, MAX, GEN) (uniform_int_distribution<int>(MIN, MAX)(GEN))

	using c = correlation;
	using uid = uniform_int_distribution<int>;

	// Gerador de números aleatórios
	default_random_engine generator(
			chrono::system_clock::now().time_since_epoch().count());

	vector<int> capacities;

	vector<Item> items;

	if (correlated == c::NONE) {

		uid distribution(MIN, MAX);

		for (int i = 0; i < nItems; i++) {

			int weight = distribution(generator);
			int profit = distribution(generator);

			items.push_back(Item(weight, profit));
		}
	} else if (correlated == c::WEAK) {

		uid distribution(MIN, MAX);

		for (int i = 0; i < nItems; i++) {
			int weight = distribution(generator);

			int profit = RAND(std::max((weight - 100), MIN), weight + 100, generator);

			items.push_back(Item(weight, profit));
		}
	}

	double weightSum = 0.0;

	for (int i = 0; i < nItems; ++i) {
		weightSum += items[i].weight();
	}

	if (similar) {

		double weightAux = 0.5 * weightSum;
		int lastCapacity = weightAux;

		for (int i = 0; i < nKnapsacks - 1; i++) {

			double ck = 0;

			for (int k = 0; k < i; k++) {
				ck += capacities[k];
			}
			
			uid distribution(MIN, std::max((int)(MIN + weightAux - ck), MIN));

			int c = distribution(generator);

			capacities.push_back(c);
			if ((lastCapacity - c) < MIN)
			lastCapacity = ((lastCapacity - c) < MIN) ? MIN : lastCapacity - c;
		}
		capacities.push_back(lastCapacity);

	} else {

		double minBound = 0.4 * weightSum / nKnapsacks;
		double maxBound = 0.6 * weightSum / nKnapsacks;
		double lastCapacity = 0.5 * weightSum;
		
		uid distribution(minBound, maxBound);

		for (int i = 0; i < nKnapsacks - 1; i++) {
			capacities.push_back(distribution(generator));

			lastCapacity -= capacities[i];
		}

		capacities.push_back(lastCapacity);
	}

	if (save) {
		saveToFile("../instances/instance" + std::to_string(index) + ".txt", nItems, nKnapsacks, capacities, items);
	}

#undef RAND

	return new Instance(items, capacities);
}

bool InstanceUtils::saveToFile(const string& filename, size_t nItems,
		size_t nKnapsacks, const vector<int>& capacities,
		const std::vector<Item>& items) {

	ofstream file(filename.c_str());

	if (!file.is_open()) {
		std::cerr << "Error opening file\n";
		return false;
	}

	file << nItems << " " << nKnapsacks << "\n";

	for (const auto& i : items) {
		file << i.weight() << " " << i.profit() << "\n";
	}

	for (const auto& c : capacities) {
		file << c << "\n";
	}

	file.close();

	return false;
}

bool InstanceUtils::isValidSolution(const Solution& _solution) {

	auto& capacities = _solution.getInstance()->getCapacities();
	auto& items = _solution.getInstance()->getItems();
	auto& solution = _solution.getSolutionVector();

	int totalWeights[capacities.size()];

	std::fill(totalWeights, totalWeights + capacities.size(), 0);

	for (int i = 0; i < solution.size(); i++) {
		if (solution[i] >= 0) {
			totalWeights[solution[i]] += items[i].weight();
		}
	}

	std::cout << "*********************************************\n";
	for (auto& c : capacities) {
		std::cout << c << " ";
	}
	std::cout << "\n";

	for (auto& c : totalWeights) {
		std::cout << c << " ";
	}
	std::cout << "\n";
	std::cout << "*********************************************\n";

	for (int i = 0; i < capacities.size(); i++) {
		if (totalWeights[i] > capacities[i]) {
			return false;
		}
	}

	return true;
}

Instance* InstanceUtils::readFromFile(const std::string& filename) {

	using namespace std;

	FILE* file = fopen(filename.c_str(), "r");

	if (!file) {
		cerr << "Error opening file!\n";
		return nullptr;
	}

	size_t nItems;
	size_t nKnapsacks;

	std::vector<Item> items;
	std::vector<int> capacities;

	fscanf(file, "%ld %ld", &nItems, &nKnapsacks);
	fgetc(file);

	for (int i = 0; i < nItems; ++i) {
		int weight;
		int profit;

		int read = fscanf(file, "%d %d", &weight, &profit);

		if (read != 2) {
			cerr << "Invalid file\n";
			fclose(file);
			return nullptr;
		}

		items.push_back(Item(weight, profit));

		fgetc(file);
	}

	for (int i = 0; i < nKnapsacks; ++i) {
		int capacity;

		int read = fscanf(file, "%d", &capacity);

		if (read != 1) {
			cerr << "Invalid file\n";
			fclose(file);
			return nullptr;
		}

		capacities.push_back(capacity);

		fgetc(file);
	}

	fclose(file);

	return new Instance(items, capacities);
}

}

