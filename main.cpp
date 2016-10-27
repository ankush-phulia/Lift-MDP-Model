#include <iostream>

using namespace std;

// Store the state space
	std::bitset<126> mystate;

int get_location(int n) {
	int location;
	location = mystate[35+n*8] + 2*mystate[36+n*8] + 4*mystate[37+n*8];
	return location;
}

int get_waiting_time(int n) {
	int time;
	time = mystate[31+n*8] + 2*mystate[32+n*8] + 4*mystate[33+n*8] + 8*mystate[34+n*8];
	return time;
}

bool does_person_exist(int n) {
	if(mystate[30+n*8]) 
		return true;
	else
		return false;
}

bool floor_button_pressed(int n) {
	if(mystate[n+21])
		return true;
	else
		return false;
}

bool elevator_button_pressed(int n, int k) {
	if(k == 1) {
		if(mystate[11+n])
			return true;
		else
			return false;
	}
	else {
		if(mystate[16+n])
			return true;
		else
			return false;
	}
}

bool doors_opened(int k) {
	if(k == 1) {
		if(mystate[10]) 
			return true;
		else
			return false;
	}
	else {
		if(mystate[11]) 
			return true;
		else
			return false;
	}
}

int get_direction(int k) {
	int direction;
	if(k == 1) {
		if(mystate[6])
			direction = -1;
		else if(mystate[7])
			direction = 1;
		else
			direction = 0;
	}
	else {
		if(mystate[8])
			direction = -1;
		else if(mystate[9])
			direction = 1;
		else
			direction = 0;
	}
	return direction;
}

int get_floor(int k) {
	int floor;
	if(k == 1) {
		floor = mystate[0] + mystate[1]*2 + mystate[2]*4;
	}
	else {
		floor = mystate[3] + mystate[4]*2 + mystate[5]*4;
	}
	return floor;
}

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

	return 0;
}