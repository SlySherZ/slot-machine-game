#include <iostream>
#include <string>
#include <time.h>
#include <unordered_map>
#include <vector>
#include "slot_machine.h"

using namespace std;

const int 	maximum_bet_allowed = 10,
			minimum_bet_allowed = 1,
			number_of_cylinders = 3,
			starting_credits = 10000;

// Should contain each letter from A-F at least once
// Should have 12 elements
vector<char> sequence = {
	'A', 'B', 'C', 'C', 'D', 'D', 'E', 'E', 'E', 'F', 'F', 'F'
};

// Should contain values for the letters from A-F, all different
unordered_map<char, int> prizes = {
	{'A', 200},
	{'B', 100},
	{'C', 50},
	{'D', 25},
	{'E', 10},
	{'F', 5}
};

// Possible ways to say yes or no
vector<string> 	yes = {"S", "s", "Sim", "sim"},
				no = {"N", "n", "Nao", "nao", "Não", "não"};

// Asks the player if he/she wants to keep playing. Returns true for 'yes'
bool continue_playing () {
	cout << "\n";

	// Try until we get a valid answer
	while (true) {
		cout << "Jogar de novo? (S/N) -> ";

		string input;
		getline(cin, input);
		if (find(yes.begin(), yes.end(), input) != yes.end()) {
			// Input was yes
			return true;
		} else if (find(no.begin(), no.end(), input) != no.end()) {
			// Input was no
			return false;
		}

		// Invalid input, try again
		cout << "Desculpe, não percebi. ";
	}
}

// Checks if the player has enough credits to keep playing
bool can_keep_playing (int credits, int min) {
	if (credits < min) {
		cout << "\nCarteira: vazia. O jogo terminou :(";
		return false;
	}

	return true;
}

// Asks the player to input the amount he wants to bet, from a minimum to
// a maximum amount inclusive. Returns the amount
int bet (int credits, int min, int max) {
	while (true) {
		cout << "Aposta (" << min << " a " << max << ") -> ";
		string input;
		getline(cin, input);

		try {
			int bet = stoi(input, nullptr, 10);

			if (bet > credits) {
				cout << "Não tem créditos suficientes. ";
			} else if (bet > max) {
				cout << "Hmm, esta máquina não dá troco. ";
			}
			else if (bet < min) {
				cout << "De preferência sem batota :P. ";
			}
			else {
				return bet;
			}

		} catch (...) {
			// Invalid input, try again
			cout << "Desculpe, não percebi. ";
		}
	}
}

// Displays the state of the cylinders on the slot machine
void show_result (vector<char> symbols) {
	cout << "\nResultado: ";
	for (auto it = symbols.begin(); it != symbols.end(); it++) {
		cout << " " << *it;
	}

}

// Displays the amount of credits the player has
void show_wallet (int credits) {
	cout << "\nCarteira: " << credits << "\n";
}

// Displays the prize awarded to the player after the last play
void show_earnings (Play_Result res) {
	cout 	<< "\nPrémio: " << res.credits_won << " (" << res.credits_multiplier
			<< " x " << res.credits_wagered << ")";
}

// Initializes the machine and handles the game loop
void start_game () {
	Slot_Machine<char> machine(
		sequence,
		prizes,
		number_of_cylinders,
		minimum_bet_allowed,
		maximum_bet_allowed
	);

	cout << "*** FMQ SLOT MACHINE ***\n";

	int credits = starting_credits;
	show_wallet(credits);

	bool playing = true;
	while (playing) {
		int amount = bet(credits, minimum_bet_allowed, maximum_bet_allowed);

		Play_Result result = machine.play(amount);
		credits += -amount + result.credits_won;

		if (result.valid) {
			show_result(machine.get_current_symbols());
			show_earnings(result);
		} else {
			// This should never happen, but still...
			cout << "\nTentou jogar um valor inválido. \
					Os créditos foram devolvidos";
		}

		if (!can_keep_playing(credits, minimum_bet_allowed))
			break;

		show_wallet(credits);
		playing = continue_playing();
	}

	cout << "\nVolte sempre :)\n";
}

int main () {
	// Use a random seed
	srand(time(NULL));

	start_game();
}




/* END */
