#pragma once

#include "g_rand.hpp"
#include "stats.hpp"

namespace sim {

	class Drops {
	public:
		Drops() = delete;
		Drops(rng::G_Rand& rand) : rand_{ rand }, chance_{ 3 } {}
		Drops(rng::G_Rand& rand, int chance) : rand_{ rand }, chance_ { chance } {}

		size_t Sim(size_t n);
	private:
		rng::G_Rand& rand_;
		int chance_;
	};

};
