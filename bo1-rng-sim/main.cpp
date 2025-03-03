#include "g_rand.hpp"
#include "drops.hpp"
#include "stats.hpp"
#include "box.hpp"

#include <iostream>
#include <array>
#include <string>
#include <span>
#include <map>

#include <iomanip> // For std::fixed and std::setprecision
#include <sstream>
#include <conio.h>
#include <chrono>

double BoxHits(size_t num_hits, const std::string& map, const std::string& goal_weapon, bool mule, bool give_weapons);
double FirstBox(size_t num_repetitions, const std::string& map, std::span<std::string> goal_weapons);
std::vector<size_t> Trades(size_t num_repetitions, const std::string& map, const std::string& goal_weapon, bool mule);
void SimulateTradeGames(size_t total_games, size_t total_trades, double good_game_avg, const char* map, const char* weapon, bool mule);
void setConsoleLine(int line, const std::string& text);

int main() {
	SimulateTradeGames(
		10000000, // total games
		150, // total trades in a game
		18.50, // whats a good weapon avg ?
		"ascension",
		"tgun",
		true // mule kick
	);
	setConsoleLine(7, "Simulations complete, press any key to exit...");
	_getch(); // Waits for any key press
	return 0;
}

/* simulates a full asc game */
void SimulateTradeGames(size_t total_games, size_t total_trades, double good_game_avg, const char* map, const char* weapon, bool mule)
{
	std::cout << "Running " << total_games << " total game simulations with " << total_trades << " trades total on " << map << " for the " << weapon << "\n";

	auto start_time = std::chrono::high_resolution_clock::now();

	std::vector<double> avg_results;
	int good_games = 0;
	int bad_games = 0;
	
	for (size_t sim_num = 0; sim_num < total_games; sim_num++)
	{
		const auto results = Trades(total_trades, map, weapon, mule);

		size_t weapons_gotten = total_trades;
		size_t result_hits = 0;
		for (size_t result : results)
		{
			result_hits += result;
		}

		// Calculate elapsed time
		auto current_time = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();

		// Calculate estimated remaining time
		double progress = static_cast<double>(sim_num + 1) / total_games;
		double estimated_total_time = elapsed / progress;
		int remaining_time = static_cast<int>(estimated_total_time - elapsed);

		// Breakdown remaining time into days, hours, minutes, and seconds
		int days = remaining_time / (24 * 3600);
		remaining_time %= 24 * 3600;
		int hours = remaining_time / 3600;
		remaining_time %= 3600;
		int minutes = remaining_time / 60;
		int seconds = remaining_time % 60;

		// Format the progress and remaining time
		std::ostringstream progress_stream, time_stream, formatted_good_avg;
		progress_stream << std::fixed << std::setprecision(2) << progress * 100;

		// Format remaining time
		if (days > 0) {
			time_stream << days << "d ";
		}
		if (hours > 0) {
			time_stream << hours << "h ";
		}
		if (minutes > 0) {
			time_stream << minutes << "m ";
		}
		if (seconds > 0 || (days == 0 && hours == 0 && minutes == 0)) {
			// Include seconds if no larger units exist
			time_stream << seconds << "s";
		}

		// Display progress and remaining time
		std::string sims_complete = std::to_string(sim_num + 1) + "/" + std::to_string(total_games) +
			" (" + progress_stream.str() + "%) - Est. Time Remaining: " + time_stream.str();
		setConsoleLine(3, "Simulations complete: " + sims_complete + "\n");

		// calculate the trade average for the game
		double trade_avg = static_cast<double>(result_hits) / weapons_gotten;
		formatted_good_avg << std::fixed << std::setprecision(2) << good_game_avg;

		// separate what is good game vs a bad one
		if (trade_avg <= good_game_avg) {
			good_games++;
		}
		else {
			bad_games++;
		}

		setConsoleLine(4, "Games with an average of " + formatted_good_avg.str() + " or below: " + std::to_string(good_games));
		setConsoleLine(5, "Games with an average above " + formatted_good_avg.str() + ": " + std::to_string(bad_games));
	}
}

double BoxHits(size_t num_hits, const std::string& map, const std::string& weapon, bool mule, bool give_weapons)
{
	rng::G_Rand rand;
	sim::Box box(rand, map);

	size_t x = box.Simulate(num_hits, weapon, mule, give_weapons);

	std::cout << "n = " << num_hits << "\n";
	std::cout << "x = " << x << "\n";
	std::cout << "average = " << (double)num_hits/(double)x << "\n";

	return (double)num_hits / (double)x;
}

/* assumes you have room in inventory for 2 box weapons */
double FirstBox(size_t num_repetitions, const std::string& map, std::span<std::string> goal_weapons) {
	rng::G_Rand rand;
	sim::Box box(rand, map);

	size_t count = 0;
	for (size_t i = 0; i < num_repetitions; i++) {
		count += box.SimulateFirstBox(goal_weapons, true);
	}

	std::cout << "n = " << num_repetitions << "\n";
	std::cout << "x = " << count << "\n";
	std::cout << "chance = " << ((double)count / (double)num_repetitions) * 100.0 << "%" << "\n";

	return (double)count / (double)num_repetitions;
}

/* assumes you start trade with a wall weapon */
std::vector<size_t> Trades(size_t num_repetitions, const std::string& map, const std::string& goal_weapon, bool mule)
{
	rng::G_Rand rand;
	sim::Box box(rand, map);

	std::vector<size_t> results(num_repetitions);

	for (size_t i = 0; i < num_repetitions; i++) {
		results[i] = box.SimulateTrade(goal_weapon, mule, true, true);
	}

	return results;
}

void setConsoleLine(int line, const std::string& text)
{
	std::cout << "\033[" << line << ";1H";
	std::cout << text << std::flush;
}