#include <iostream>

using namespace std;

// Store the state space
	std::bitset<126> mystate;

// FUNCTIONS TO UPDATE THE STATE OF VARIABLES
void update_floor(int n, int k) {
	for(int i = 2; i >= 0; i--) {
		mystate[-3+k*3+i] = n%2;
		n = n/2;
	}
}

void elevator_moves(int dir, int k) {
	bool down = false, up = false;
	int index = 4 + 2*k;
	if(dir == 1)
		up = true;
	else if(dir == -1)
		down = true;
	mystate[index] = down;
	mystate[index+1] = up;
}

void elevator_doors_open(bool open, int k) {
	if(open)
		mystate[9+k] = true;
	else
		mystate[9+k] = false;
}

void lift_button(int n, int k, bool pressed) {
	if(pressed) 
		mystate[6+5*k+n] = true;
	else
		mystate[6+5*k+n] = false;
}

void floor_button(int n, bool pressed) {
	if(pressed) 
		mystate[21+n] = true;
	else
		mystate[21+n] = false;
}

void add_person(int n, int floor) {
	mystate[30+n*8] = true;
	for(int i = 0; i < 4; i++) {
		mystate[31+n*8+i] = false;
	}
	for(int i = 2; i >= 0; i--) {
		mystate[35+n*8+i] = floor%2;
		floor = floor/2;
	}
}

void delete_person(int n) {
	for(int i = 0; i < 8; i++) {
		mystate[30+n*8+i] = false;
	}
}

void person_moves(int n, int loc) {
	for(int i = 2; i >= 0; i--) {
		mystate[35+n*8+i] = loc%2;
		loc = loc/2;
	}
}

void update_person_time(int n) {
	int time = get_waiting_time(n);
	time++;
	for(int i = 3; i >= 0; i--) {
		mystate[31+n*8+i] = time%2;
		time = time/2;
	}
}

// FUNCTIONS TO GET STATE OF VARIABLES
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