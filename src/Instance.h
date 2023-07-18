#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <cstddef>
#include <vector>

#include "Item.h"

namespace cfpmm {

class Solution;
class Ant;
class Colony;

class Instance {
public:
	/**
	 * @param nItems Num de itens
	 * @param nKnapsacks Num de mochilas
	 * @param items Lista de itens
	 * @param capacities Lista de mochilas
	 */
	Instance(const std::vector<Item>& items,
			const std::vector<int>& capacities);

	virtual ~Instance();

	/**
	 * @return List of items.
	 */
	const std::vector<Item>& getItems() const {
		return items;
	}

	/**
	 * @return List of capacities.
	 */
	const std::vector<int>& getCapacities() const {
		return capacities;
	}

	/**
	 * @return Number of items.
	 */
	std::size_t getNumItems() const {
		return nItems;
	}

	/**
	 * @return Number of knapsacks.
	 */
	std::size_t getNumKnapsacks() const {
		return nKnapsacks;
	}

private:

	/**
	 * Number of items.
	 */
	std::size_t nItems;

	/**
	 * Number of knapsacks.
	 */
	std::size_t nKnapsacks;

	/**
	 * List of items.
	 */
	std::vector<Item> items;

	/**
	 * List of capaticies.
	 */
	std::vector<int> capacities;

	friend class Solution;
	friend class Ant;
	friend class Colony;
};

}

#endif /* INSTANCE_H_ */
