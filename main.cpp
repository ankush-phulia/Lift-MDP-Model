#include <iostream>

using namespace std;

// No of floors and the number of elevators 
int P, Q, R; // R  = log P rounded up

// Create a class of states
class state {
	// state space
	std::bitset<200> mystate;
	// Link to states
	vector<state> links;

	state(state new_state) {
		for(int i = 0; i < 200; i++) {
			mystate[i] = new_state.mystate[i];
		}
	}
}initial_state, current_state;

// CREATE NEW STATES
void create_state(vector<string> actions, state current_state) {
	state new_state(current_state);

	for(int i = 0; i < actions.size(); i++) {
		// A PERSON BEING ADDED
		if(action[i][0] == 'P') {
			if(action[i][1] == 'I') {
				int person = (int)action[i][2] -'0';
				int floor = (int)action[i][3] - '0';
				add_person(person, floor, new_state.mystate);
			}
			else if(action[i][1] == 'O') {
				int person = (int)action[i][2] - '0';
				delete_person(person, new_state.mystate)
			}
			else if(action[i][1] == 'D') {
				int person = (int)action[i][2] - '0';
				int destination = (int)action[i][3] - '0';
				update_person_destination(person, destination, new_state.mystate);
			}
			else if(action[i][1] == 'L') {
				int person = (int)action[i][2] - '0';
				int location = (int)action[i][3] - '0';
				person_moves(person, location, new_state.mystate);
			}
		}

		// A BUTTON BEING PRESSED
		if(action[i][0] == 'B') {
			if(action[i][1] == 'U') {
				int floor = (int)action[i][2] - '0';
				floor_button(2*floor-1, true, new_state.mystate);
			}
			else if(action[i][1] == 'D') {
				int floor = (int)action[i][2] - '0';
				floor_button(2*(floor-1), true, new_state.mystate);
			}
			else {
				int floor = (int)action[i][1] - '0';
				int elevator = (int)action[i][2] - '0';
				lift_button(floor, elevator, true, new_state.mystate);
			}
		}

		// LIFT MOVING
		if(action[i][0] == 'A') {
			if(action[i][1] == 'U') {
				int elevator = (int)action[i][2] -'0';
				elevator_moves(1, elevator, new_state.mystate);
			}
			else if(action[i][1] == 'D') {
				int elevator = (int)action[i][2] -'0';
				elevator_moves(-1, elevator, new_state.mystate);
			}
			else if(action[i][1] == 'S') {
				int elevator = (int)action[i][2] -'0';
				elevator_moves(0, elevator, new_state.mystate);
			}
			else if(action[i][1] == 'O') {
				int elevator = (int)action[i][3] -'0';
				if(action[i][2] == 'U') {
					elevator_moves(1, elevator, new_state.mystate);
				}
				else if(action[i][2] == 'D') {
					elevator_moves(-1, elevator, new_state.mystate);
				}
				elevator_doors(true, elevator, new_state.mystate);
			}
			else if(action[i][1] == 'C') {
				int elevator = (int)action[i][3] -'0';
				if(action[i][2] == 'U') {
					elevator_moves(1, elevator, new_state.mystate);
				}
				else if(action[i][2] == 'D') {
					elevator_moves(-1, elevator, new_state.mystate);
				}
				elevator_doors(false, elevator, new_state.mystate);
			}
			else {
				int elevator = (int)action[i][1] -'0';
				int floor = (int)action[i][2] -'0';
				update_floor(floor, elevator, new_state.mystate);
			}
		}
	}
	for(int i = 0; i < no_of_people; i++) {
		if(new_state.mystate[10+4*P+n*11])
			update_person_time(i, new_state.mystate);
	}
	current_state.links.push_back(new_state);
}

// PERFORM ACTION
void perform_action(string action, state current_state) {
	state new_state(current_state);

	
}

// FUNCTIONS TO UPDATE THE STATE OF VARIABLES
void update_floor(int n, int k, bitset<200> mystate) {
	for(int i = 2; i >= 0; i--) {
		mystate[-3+k*3+i] = n%2;
		n = n/2;
	}
}

void elevator_moves(int dir, int k, bitset<200> mystate) {
	bool down = false, up = false;
	int index = 4 + 2*k;
	if(dir == 1)
		up = true;
	else if(dir == -1)
		down = true;
	mystate[index] = down;
	mystate[index+1] = up;
}

void elevator_doors(bool open, int k, bitset<200> mystate) {
	if(open)
		mystate[9+k] = true;
	else
		mystate[9+k] = false;
}

void lift_button(int n, int k, bool pressed, bitset<200> mystate) {
	if(pressed) 
		mystate[6+P*k+n] = true;
	else
		mystate[6+P*k+n] = false;
}

void floor_button(int n, bool pressed, bitset<200> mystate) {
	if(pressed) 
		mystate[11+2*P+n] = true;
	else
		mystate[11+2*P+n] = false;
}

void add_person(int n, int floor, bitset<200> mystate) {
	mystate[10+4*P+n*11] = true;
	for(int i = 0; i < 4; i++) {
		mystate[11+4*P+n*11+i] = false;
	}
	for(int i = 2; i >= 0; i--) {
		mystate[15+4*P+n*11+i] = floor%2;
		floor = floor/2;
	}
}

void delete_person(int n, bitset<200> mystate) {
	for(int i = 0; i < 11; i++) {
		mystate[10+4*P+n*11+i] = false;
	}
}

void person_moves(int n, int loc, bitset<200> mystate) {
	for(int i = 2; i >= 0; i--) {
		mystate[15+4*P+n*11+i] = loc%2;
		loc = loc/2;
	}
}

void update_person_time(int n, bitset<200> mystate) {
	int time = get_waiting_time(n);
	time++;
	for(int i = 3; i >= 0; i--) {
		mystate[11+4*P+n*11+i] = time%2;
		time = time/2;
	}
}

void update_person_destination(int n, int floor, bitset<200> mystate) {
	for(int i = 2; i >= 0; i--) {
		mystate[18+4*P+n*11+i] = floor%2;
		floor = floor/2;
	}
}

// FUNCTIONS TO GET STATE OF VARIABLES
int get_destination(int n, bitset<200> mystate) {
	int destination;
	destination = mystate[18+4*P+n*11] + 2*mystate[19+4*P+n*11] + 4*mystate[20+4*P+n*11];
	return destination;
}

int get_location(int n, bitset<200> mystate) {
	int location;
	location = mystate[15+4*P+n*11] + 2*mystate[16+4*P+n*11] + 4*mystate[17+4*P+n*11];
	return location;
}

int get_waiting_time(int n, bitset<200> mystate) {
	int time;
	time = mystate[11+4*P+n*11] + 2*mystate[12+4*P+n*11] + 4*mystate[13+4*P+n*11] + 8*mystate[14+4*P+n*11];
	return time;
}

bool does_person_exist(int n, bitset<200> mystate) {
	if(mystate[10+4*P+n*11]) 
		return true;
	else
		return false;
}

bool floor_button_pressed(int n, bitset<200> mystate) {
	if(mystate[n+11+2*P])
		return true;
	else
		return false;
}

bool elevator_button_pressed(int n, int k, bitset<200> mystate) {
	if(k == 1) {
		if(mystate[11+n])
			return true;
		else
			return false;
	}
	else {
		if(mystate[11+P+n])
			return true;
		else
			return false;
	}
}

bool doors_opened(int k, bitset<200> mystate) {
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

int get_direction(int k, bitset<200> mystate) {
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

int get_floor(int k, bitset<200> mystate) {
	int floor;
	if(k == 1)
		floor = mystate[0] + mystate[1]*2 + mystate[2]*4;
	else
		floor = mystate[3] + mystate[4]*2 + mystate[5]*4;
	return floor;
}

int main(int argc, char* argv[]) {
	cout<<"Lets get started Ankush!";
	// State Space will be a bitset or a string of bits for smaller storage and easy accessability
		// We need to decide the size of our bitset
		  // ELEVATOR SPACE (Each 3P+logP+1 bits)
			// log P bits for floor number of elevator
			// 2*P - 2 bits for buttons on each floor
			// P bits for buttons on elevator
			// 2 bits for direction of elevator 1
			// 1 bit for opening and closing of doors of elevator 1
		  // PERSON SPACE - Lets assume that there are a total of 12 people in the whole system (Each 11 bits)
			// 1 bit to show presence in system
			// 4 bits to hold wait time
			// 3 bits to hold location
			// 3 bits to hold destination
	P = argv[1];
	string input;
	do {
		cin>>input;
		vector<string> input_actions;
		for(int i = 0; i < input_actions.size(); i++) {
			if(input_actions[i] != "0")
				perform_action(input_actions[i]);
		}
	} while(input != "DONE");
	
	return 0;
}