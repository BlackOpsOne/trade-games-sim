#include "drops.hpp"

namespace sim {

	size_t Drops::Sim(size_t n) {
		size_t drops_given = 0;

		for (size_t i = 0; i < n; i++) {
			if (rand_.RandomInt(100) > (chance_ - 1)) {
				continue;
			}

			drops_given++;
		}

		return drops_given;
	}

};