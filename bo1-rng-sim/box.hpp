#pragma once

#include "g_rand.hpp"
#include <string>
#include <span>

namespace sim {
	int def_weighting();

	struct Weapon {
		Weapon() : name{ "" }, weighting{ def_weighting } {}
		Weapon(const std::string& name) : name{ name }, weighting{ def_weighting } {}
		Weapon(const std::string& name, int (*weighting)(void)) : name{ name }, weighting{ weighting } {}

		std::string name;
		int (*weighting)(void);

		bool operator==(const std::string& rhs) const noexcept
		{
			return this->name == rhs;
		}
	};

	class Box {
	public:
		Box() = delete;
		Box(rng::G_Rand& rand, std::string mapname);

		size_t Simulate(size_t hits, const std::string& goal_weapon, bool mule, bool give_weapons);
		size_t SimulateFirstBox(std::span<std::string> goal_weapons, bool mule);
		size_t SimulateTrade(const std::string& goal_weapon, bool mule, bool give_weapons, bool wall_weapon);

		static std::vector<Weapon> GenerateWeaponList(const std::string& mapname);
	public:
		rng::G_Rand& rand_;
		std::array<std::string, 4> current_weapons_{};
		std::vector<Weapon> weapons_;

		size_t current_chest_hits_{ 0 };
		bool movable_{ true };
		bool first_box_{ false };

		std::string SimHit();
		std::string BoxHit();
		void SetWeaponEmpty(size_t slot);
		void SetWeaponEmpty(const std::string& name);

		[[nodiscard]] bool ShouldBoxMove();

		[[nodiscard]] static bool IsTactical(const std::string& name);
	};

};