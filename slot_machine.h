#include <algorithm>
#include <unordered_map>
#include <vector>
#include "cylinder.h"

struct Play_Result {
	bool valid;					// Did the play succeed?
	int credits_wagered,		// Amount of credits wagered
		credits_multiplier,		// Prize multiplier for the play result
		credits_won;			// Credits awarded to the player
};

/**
 *  Simulates a slot machine game
 */
template <typename T>
class Slot_Machine {
	std::vector<Cylinder<T>> cylinders;
	std::unordered_map<T, int> prizes;

	int min_amount,
		max_amount;

public:
	Slot_Machine (
		std::vector<T> sequence,
		std::unordered_map<T, int> prizes,
		int number_of_cylinders,
		int min_bet_amount,
		int max_bet_amount
	)
	: 	prizes(prizes),
		min_amount(min_bet_amount),
		max_amount(max_bet_amount)
	{

		for (int i = 0; i < number_of_cylinders; i++) {
			cylinders.push_back(make_cylinder(sequence));
		}
	}

	// Plays a given amount of credit. Returns a struct with the results of the
	// play. Check 'struct Play_Result'
	Play_Result play (int amount) {
		struct Play_Result result;
		result.credits_wagered = amount;
		result.valid = true;

		// Just in case someone tries to cheat
		if (amount < min_amount || amount > max_amount) {
			// Give the player his money back
			result.credits_won = amount;
			result.credits_multiplier = 1;	// For consistency
			result.valid = false;

			return result;
		}

		// Spin the cylinders
		for (auto it = cylinders.begin(); it != cylinders.end(); it++) {
			int random_rotation = random_number(0, it->size());
			it->spin(random_rotation);
		}

		// Fill results
		result.credits_multiplier = get_prize_multiplier();
		result.credits_won = result.credits_multiplier * result.credits_wagered;

		return result;
	}

	// Returns a list with the current cylinder positions
	std::vector<T> get_current_symbols () {
		std::vector<T> symbols;
		for (auto it = cylinders.begin(); it != cylinders.end(); it++) {
			symbols.push_back(it->get_symbol());
		}

		return symbols;
	}


private:
	// Computes a random number between min(inclusive) and max(exclusive)
	int random_number (int min, int max) {
		if (max <= min) {
			return 0;
		}

		int difference = max - min;
		return min + rand() % difference;
	}

	// Creates a cylinder, by randomizing the given sequence
	Cylinder<T> make_cylinder (std::vector<T> seq) {
		std::vector<T> sequence(seq);
		random_shuffle(sequence.begin(), sequence.end());

		return Cylinder<T>(sequence);
	}

	// Computes the prize multiplier for the last play
	int get_prize_multiplier () {
		for (auto it = cylinders.begin() + 1; it != cylinders.end(); it++) {
			if (cylinders[0].get_symbol() != it->get_symbol()) {
				// Not all symbols are the same, multiplier is 0
				return 0;
			}
		}

		// Return the prize multiplier for the symbol
		return prizes[cylinders[0].get_symbol()];
	}
};
