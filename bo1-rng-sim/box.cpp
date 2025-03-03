#include "box.hpp"

#include <vector>

namespace sim {

	Box::Box(rng::G_Rand& rand, std::string mapname) : rand_{ rand } {
		if (mapname == "nacht") {
			movable_ = false;
		}

		weapons_ = GenerateWeaponList(mapname);
	}

	size_t Box::Simulate(size_t hits, const std::string& goal_weapon, bool mule, bool give_weapons)
	{
		if (give_weapons) {
			current_weapons_[0] = weapons_[0].name;
			current_weapons_[1] = weapons_[1].name;
			current_weapons_[2] = weapons_[2].name;
			for (const auto& w : weapons_) {
				if (IsTactical(w.name)) {
					current_weapons_[3] = w.name;
					break;
				}
			}
		}

		size_t count = 0;

		for (size_t i = 0; i < hits; i++) {
			if (!mule) {
				SetWeaponEmpty(2);
			}

			if (SimHit() == goal_weapon) {
				count += 1;

				SetWeaponEmpty(goal_weapon);
			}			
		}

		return count;
	}

	size_t Box::SimulateFirstBox(std::span<std::string> goal_weapons, bool mule)
	{
		first_box_ = true;

		std::vector<bool> found(goal_weapons.size());
		
		bool any_found = false;
		while (true) {
			if (mule) {
				SetWeaponEmpty(2);
				if (!any_found) {
					SetWeaponEmpty(1);
				}
			}
			else {
				SetWeaponEmpty(2);
			}

			auto rand = SimHit();

			if (rand == "") {
				break;
			}

			auto it = std::find(goal_weapons.begin(), goal_weapons.end(), rand);
			if (it != goal_weapons.end()) {
				if (!IsTactical(rand)) {
					any_found = true;
				}

				auto ind = it - goal_weapons.begin();
				found[ind] = true;
			}
		}

		for (size_t i = 0; i < 4; i++) {
			SetWeaponEmpty(i);
		}

		first_box_ = false;

		bool all_found = true;
		for (auto b : found) {
			all_found = (all_found && b);
		}
		return all_found;
	}

	size_t Box::SimulateTrade(const std::string& goal_weapon, bool mule, bool give_weapons, bool wall_weapon)
	{
		if (give_weapons) {
			current_weapons_[0] = weapons_[0].name;
			current_weapons_[1] = weapons_[1].name;
			current_weapons_[2] = weapons_[2].name;
			for (const auto& w : weapons_) {
				if (IsTactical(w.name)) {
					current_weapons_[3] = w.name;
					break;
				}
			}
		}

		if (wall_weapon) {
			SetWeaponEmpty(1);
		}

		size_t hits = 0;
		for (size_t i = 0; i < 400; i++) {
			if (!mule) {
				SetWeaponEmpty(2);
			}

			hits += 1;

			if (SimHit() == goal_weapon) {
				return hits;
			}
		}

		return hits;
	}

	std::string Box::SimHit()
	{
		std::string rand = BoxHit();

		if (ShouldBoxMove()) {
			current_chest_hits_ = 0;
			return "";
		}
		else {
			current_chest_hits_++;
		}

		if (IsTactical(rand)) {
			current_weapons_[3] = rand;
			return rand;
		}

		auto it = std::find(current_weapons_.begin(), current_weapons_.end() - 1, Weapon());
		if (it != current_weapons_.end() - 1) {
			current_weapons_[it - current_weapons_.begin()] = rand;
		}
		else {
			current_weapons_[0] = rand;
		}

		return rand;
	}

	std::string Box::BoxHit() {
		rand_.CycleRand(39);

		std::vector<Weapon> filtered_weapons;

		for (const auto& weapon : weapons_) {
			if (std::find(current_weapons_.begin(), current_weapons_.end(), weapon) == current_weapons_.end()) {
				size_t n = weapon.weighting();
				for (size_t i = 0; i < n; i++) {
					filtered_weapons.push_back(weapon);
				}
			}
		}

		rand_.array_randomize(filtered_weapons);
		return filtered_weapons[rand_.RandomInt(filtered_weapons.size())].name;
	}

	void Box::SetWeaponEmpty(size_t slot)
	{
		current_weapons_[slot] = "";
	}

	void Box::SetWeaponEmpty(const std::string& name)
	{
		auto it = std::find(current_weapons_.begin(), current_weapons_.end(), name);
		if (it != current_weapons_.end()) {
			current_weapons_[it - current_weapons_.begin()] = "";
		}
	}

	bool Box::ShouldBoxMove()
	{
		if (!movable_ || current_chest_hits_ < 4) {
			return false;
		}

		if (first_box_ && current_chest_hits_ >= 8) {
			return true;
		}

		int random = rand_.RandomInt(100);

		bool move = false;

		if (current_chest_hits_ >= 4 && current_chest_hits_ < 8) {
			move = (random < 15);
		}
		else if (current_chest_hits_ >= 8 && current_chest_hits_ < 13) {
			move = (random < 30);
		}
		else if (current_chest_hits_ >= 13) {
			move = (random < 50);
		}

		return move;
	}

	bool Box::IsTactical(const std::string& name)
	{
		if (name == "monkey") {
			return true;
		}

		if (name == "gersch") {
			return true;
		}

		if (name == "dolls") {
			return true;
		}

		return false;
	}

	std::vector<Weapon> Box::GenerateWeaponList(const std::string& mapname)
	{
		if (mapname == "nacht") {
			return std::vector<Weapon>{
				{ "python" },
				{ "cz75" },
				{ "g11" },
				{ "famas" },
				{ "spectre" },
				{ "cz75dw" },
				{ "spas" },
				{ "hs10" },
				{ "aug" },
				{ "galil" },
				{ "commando" },
				{ "fal" },
				{ "dragunov" },
				{ "l96a1" },
				{ "rpk" },
				{ "hk21" },
				{ "m72" },
				{ "chinalake" },
				{ "monkey" },
				{ "ray" },
				{ "bow" },
				{ "knife" },
				{ "tgun" }
			};
		}

		if (mapname == "ascension") {
			return std::vector<Weapon>{
				{ "python" },
				{ "cz75" },
				{ "g11" },
				{ "famas" },
				{ "spectre" },
				{ "cz75dw" },
				{ "spas" },
				{ "hs10" },
				{ "aug" },
				{ "galil" },
				{ "commando" },
				{ "fal" },
				{ "dragunov" },
				{ "l96a1" },
				{ "rpk" },
				{ "hk21" },
				{ "m72" },
				{ "chinalake" },
				{ "gersch" },
				{ "dolls" },
				{ "ray" },
				{ "bow" },
				{ "knife" },
				{ "tgun" }
			};
		}

		if (mapname == "riese") {
			return std::vector<Weapon>{
				{ "python" },
				{ "cz75" },
				{ "g11" },
				{ "famas" },
				{ "spectre" },
				{ "cz75dw" },
				{ "spas" },
				{ "hs10" },
				{ "aug" },
				{ "galil" },
				{ "commando" },
				{ "fal" },
				{ "dragunov" },
				{ "l96a1" },
				{ "rpk" },
				{ "hk21" },
				{ "m72" },
				{ "chinalake" },
				{ "monkey" },
				{ "ray" },
				{ "bow" },
				{ "knife" },
				{ "waffe" }
			};
		}
		return std::vector<Weapon>();
	}

	int def_weighting() {
		return 1;
	}
};