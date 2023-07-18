#include <ctime>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <pthread.h>
#include <thread>
#include <string>
#include <fstream>

#include "Debug.h"
#include "Colony.h"
#include "Instance.h"
#include "InstanceUtils.h"
#include "Item.h"
#include "Solution.h"

using namespace std;

int main(void) {

	using namespace cfpmm;

	/*================== GERA AS 10 INSTÂNCIAS =====================
	
	 for (int i = 1; i < 11; ++i) {
	 InstanceUtils::generate(i, 10 , i, correlation::WEAK, true);
	std::cerr << "Generate Instance \n";
	 }
	
	*/
// ===============================================================

	// ========================== Teste ==============================

	std::ofstream file("../results/results.csv");

	if (!file.is_open()) {
		std::cerr << "Error opening file\n";
		return false;
	}

	file
			<< "nItens,nKnapsacks,ColonyTime,ColonyResult,iterations\n";

	for (int i = 1; i < 11; ++i) {

		file.flush();
		auto instance = InstanceUtils::readFromFile(
				"../instances/instance" + std::to_string(i) + ".txt");

		Colony* colony = new Colony(instance, 10, 0.9, 1, 10);

		const clock_t begin_time_colony = clock();

		auto solutionColony = colony->run();

		float time_colony = float(clock() - begin_time_colony) / CLOCKS_PER_SEC;

		file << (i * 2) << "," << i << "," << time_colony << ","
				<< std::get<0>(solutionColony).getValue() << "," << std::get<1>(solutionColony)
				<< "\n";
	}

	// ===============================================================

	return 0;
}
