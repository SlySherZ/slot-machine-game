CXX = g++
FLAGS = -std=c++11
NAME = slot_machine_game

make:
	$(CXX) $(NAME).cpp -o $(NAME) $(FLAGS)

	
