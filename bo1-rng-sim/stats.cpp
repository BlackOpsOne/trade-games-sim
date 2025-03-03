#include "stats.hpp"

namespace stats {
	double binom(unsigned int n, unsigned int x, double p)
	{
		return comb(n, x) * pow(p, x) * pow(1 - p, n - x);
	}

	double binomg(unsigned int n, unsigned int x, double p)
	{
		double prob = 0.0;
		double cprob = 0.0;

		for (size_t i = x; i < n; i++) {
			cprob = binom(n, i, p);
			if (isnan(cprob)) {
				break;
			}
			prob += cprob;
		}

		return prob;
	}

	double comb(unsigned int in, unsigned int ir)
	{
		double n = static_cast<double>(in);
		double r = static_cast<double>(ir);
		return std::round(1 / ((n + 1) * std::beta(n - r + 1, r + 1)));
	}
};