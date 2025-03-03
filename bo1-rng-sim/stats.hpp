#pragma once

#include <cmath>
#include <numeric>
#include <span>
#include <algorithm>
#include <vector>

namespace stats {
	[[nodiscard]] double binom(unsigned int n, unsigned int x, double p);
	[[nodiscard]] double binomg(unsigned int n, unsigned int x, double p);

	[[nodiscard]] double comb(unsigned int in, unsigned int ir);

	template <typename T>
	double mean(std::span<T> data) {
		double sum = std::accumulate(data.begin(), data.end(), 0.0);
		return sum / data.size();
	}

	template <typename T>
	double sd(std::span<T> data) {
		double m = mean(data);
		std::vector<double> diff(data.size());
		std::transform(data.begin(), data.end(), diff.begin(),
			[m](T x) { return (x - m)*(x - m); });

		double sq_sum = std::accumulate(diff.begin(), diff.end(), 0.0);
		return std::sqrt(sq_sum / (data.size() - 1));
	}
};