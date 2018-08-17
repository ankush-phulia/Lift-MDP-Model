CC = g++
CXXFLAGS = -std=c++11 -g -Ofast -march=native -I include/

compile-main :
	$(CC) src/main.cpp src/state.cpp $(CXXFLAGS) -o mdp 

compile-bonus :
	$(CC) src/main_bonus.cpp src/node.cpp $(CXXFLAGS) -o bonus 

clear :
	rm mdp bonus