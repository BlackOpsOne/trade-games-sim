#pragma once

#include <array>
#include <vector>

namespace rng {

	class G_Rand
	{
	public:
		G_Rand();
		G_Rand(int seed) : holdrand_{ seed } {}

		[[nodiscard]] int RandomIntRange(int min, int max) { return irand(min, max); }
		[[nodiscard]] int RandomInt(int max) { return irand(0, max); }

		void CycleRand() { holdrand_ = holdrand_ * 0x343fd + 0x269ec3; }
		void CycleRand(size_t n);
		int GetSeed() { return holdrand_; }

		template <typename T, size_t n>
		void array_randomize(std::array<T, n>& original_array) {
			for (size_t i = 0; i < n; i++) {
				int j = RandomInt(n);
				std::swap(original_array[i], original_array[j]);
			}
		}

		template <typename T>
		void array_randomize(std::vector<T>& original_array) {
			for (size_t i = 0; i < original_array.size(); i++) {
				int j = RandomInt(original_array.size());
				std::swap(original_array[i], original_array[j]);
			}
		}

	private:
		int holdrand_;

		int irand(int min, int max);
	};

};