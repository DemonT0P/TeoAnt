#include "Instance.h"

namespace cfpmm {

Instance::Instance(const std::vector<Item>& _items,
		const std::vector<int>& _capacities) :
		nItems(_items.size()), 
		nKnapsacks(_capacities.size()), 
		items(_items), 
		capacities(_capacities) {

}

Instance::~Instance() {

}

}
