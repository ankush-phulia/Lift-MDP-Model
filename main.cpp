#include <iostream>

using namespace std;

int main() {
	cout<<"Lets get started Ankush!";
	// State Space will be a bitset or a string of bits for smaller storage and easy accessability
		// We need to decide the size of our bitset
		  // ELEVATOR SPACE (Total 30 Bits)
			// 3 bits for floor number of elevator 1
			// 3 bits for floor number of elevator 2
			// 8 bits for buttons on each floor ( 1 + 2 + 2 + 2 + 1 )
			// 5 bits for buttons on elevator 1
			// 5 bits for buttons on elevator 2
			// 2 bits for direction of elevator 1
			// 2 bits for direction of elevator 2
			// 1 bit for opening and closing of doors of elevator 1
			// 1 bit for opening and closing of doors of elevator 2
		  // PERSON SPACE - Lets assume that there are a total of 12 people in the whole system (Each 8 bits)
			// 1 bit to show presence in system
			// 4 bits to hold wait time
			// 3 bits to hold location

	// Store the state space
	std::bitset<126> mystate;
	return 0;
}