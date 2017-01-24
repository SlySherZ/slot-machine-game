/**
 * Stores a circular sequence and a position on the sequence.
 * Allows use to spin the sequence around and access one of the elements
 * relative to the current position.
 */
template <typename T>
class Cylinder {
	std::vector<T> sequence;
	int position = 0;

public:
	Cylinder (std::vector<T> sequence): sequence(sequence) {}

	// Spins the sequence by 'positions_moved' positions
	void spin (int positions_moved) {
		// Prevent eventual overflow
		position = (position + positions_moved) % sequence.size();
	}

	// Getter for the sequence elements relative to the current element
	T get_symbol (int ofset = 0) {
		int index = (position + ofset) % sequence.size();

		// Wrap negative index back to positive
		if (index < 0)
			index += sequence.size();

		return sequence[index];
	}

	int size () {
		return sequence.size();
	}
};
