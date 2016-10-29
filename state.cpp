#include "state.h"

state::state(state new_state){
	for(int i = 0; i < 200; i++) {
		mystate[i] = new_state.mystate[i];
	}
}

void state::create_state(vector<string> action, state current_state) {
	state new_state(current_state);

	for(int i = 0; i < action.size(); i++) {
		// A PERSON BEING ADDED
		if(action[i][0] == 'P') {
			if(action[i][1] == 'I') {
				int person = (int)action[i][2] -'0';
				int floor = (int)action[i][3] - '0';
				new_state.add_person(person, floor);
			}
			else if(action[i][1] == 'O') {
				int person = (int)action[i][2] - '0';
				new_state.delete_person(person);
			}
			else if(action[i][1] == 'D') {
				int person = (int)action[i][2] - '0';
				int destination = (int)action[i][3] - '0';
				new_state.update_person_destination(person, destination);
			}
			else if(action[i][1] == 'L') {
				int person = (int)action[i][2] - '0';
				int location = (int)action[i][3] - '0';
				new_state.person_moves(person, location);
			}
		}

		// A BUTTON BEING PRESSED
		if(action[i][0] == 'B') {
			if(action[i][1] == 'U') {
				int floor = (int)action[i][2] - '0';
				new_state.floor_button(2*floor-1, true);
			}
			else if(action[i][1] == 'D') {
				int floor = (int)action[i][2] - '0';
				new_state.floor_button(2*(floor-1), true);
			}
			else {
				int floor = (int)action[i][1] - '0';
				int elevator = (int)action[i][2] - '0';
				new_state.lift_button(floor, elevator, true);
			}
		}

		// LIFT MOVING
		if(action[i][0] == 'A') {
			if(action[i][1] == 'U') {
				int elevator = (int)action[i][2] -'0';
				new_state.elevator_moves(1, elevator);
			}
			else if(action[i][1] == 'D') {
				int elevator = (int)action[i][2] -'0';
				new_state.elevator_moves(-1, elevator);
			}
			else if(action[i][1] == 'S') {
				int elevator = (int)action[i][2] -'0';
				new_state.elevator_moves(0, elevator);
			}
			else if(action[i][1] == 'O') {
				int elevator = (int)action[i][3] -'0';
				if(action[i][2] == 'U') {
					new_state.elevator_moves(1, elevator);
				}
				else if(action[i][2] == 'D') {
					new_state.elevator_moves(-1, elevator);
				}
				new_state.elevator_doors(true, elevator);
			}
			else if(action[i][1] == 'C') {
				int elevator = (int)action[i][3] -'0';
				if(action[i][2] == 'U') {
					new_state.elevator_moves(1, elevator);
				}
				else if(action[i][2] == 'D') {
					new_state.elevator_moves(-1, elevator);
				}
				new_state.elevator_doors(false, elevator);
			}
			else {
				int elevator = (int)action[i][1] -'0';
				int floor = (int)action[i][2] -'0';
				new_state.update_floor(floor, elevator);
			}
		}
	}
	for(int i = 0; i < no_of_people; i++) {
		if(new_state.mystate[10+4*P+n*11])
			new_state.update_person_time(i);
	}
	current_state.links.push_back(new_state);
}

// PERFORM ACTION
void state::perform_action(std::string action, state current_state) {
	state new_state(current_state);
	
}

// FUNCTIONS TO GET STATE OF VARIABLES
int state::get_destination(int n) {
	int destination;
	destination = mystate[18+4*P+n*11] + 2*mystate[19+4*P+n*11] + 4*mystate[20+4*P+n*11];
	return destination;
}

int state::get_location(int n) {
	int location;
	location = mystate[15+4*P+n*11] + 2*mystate[16+4*P+n*11] + 4*mystate[17+4*P+n*11];
	return location;
}

int state::get_waiting_time(int n) {
	int time;
	time = mystate[11+4*P+n*11] + 2*mystate[12+4*P+n*11] + 4*mystate[13+4*P+n*11] + 8*mystate[14+4*P+n*11];
	return time;
}

bool state::does_person_exist(int n) {
	if(mystate[10+4*P+n*11]) 
		return true;
	else
		return false;
}

bool state::floor_button_pressed(int n) {
	if(mystate[n+11+2*P])
		return true;
	else
		return false;
}

bool state::elevator_button_pressed(int n, int k) {
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

bool state::doors_opened(int k) {
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

int state::get_direction(int k) {
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

int state::get_floor(int k) {
	int floor;
	if(k == 1)
		floor = mystate[0] + mystate[1]*2 + mystate[2]*4;
	else
		floor = mystate[3] + mystate[4]*2 + mystate[5]*4;
	return floor;
}

// FUNCTIONS TO UPDATE THE STATE OF VARIABLES
void state::update_floor(int n, int k) {
	for(int i = 2; i >= 0; i--) {
		mystate[-3+k*3+i] = n%2;
		n = n/2;
	}
}

void state::elevator_moves(int dir, int k) {
	bool down = false, up = false;
	int index = 4 + 2*k;
	if(dir == 1)
		up = true;
	else if(dir == -1)
		down = true;
	mystate[index] = down;
	mystate[index+1] = up;
}

void state::elevator_doors(bool open, int k) {
	if(open)
		mystate[9+k] = true;
	else
		mystate[9+k] = false;
}

void state::lift_button(int n, int k, bool pressed) {
	if(pressed) 
		mystate[6+P*k+n] = true;
	else
		mystate[6+P*k+n] = false;
}

void state::floor_button(int n, bool pressed) {
	if(pressed) 
		mystate[11+2*P+n] = true;
	else
		mystate[11+2*P+n] = false;
}

void state::add_person(int n, int floor) {
	mystate[10+4*P+n*11] = true;
	for(int i = 0; i < 4; i++) {
		mystate[11+4*P+n*11+i] = false;
	}
	for(int i = 2; i >= 0; i--) {
		mystate[15+4*P+n*11+i] = floor%2;
		floor = floor/2;
	}
}

void state::delete_person(int n) {
	for(int i = 0; i < 11; i++) {
		mystate[10+4*P+n*11+i] = false;
	}
}

void state::person_moves(int n, int loc) {
	for(int i = 2; i >= 0; i--) {
		mystate[15+4*P+n*11+i] = loc%2;
		loc = loc/2;
	}
}

void state::update_person_time(int n) {
	int time = get_waiting_time(n);
	time++;
	for(int i = 3; i >= 0; i--) {
		mystate[11+4*P+n*11+i] = time%2;
		time = time/2;
	}
}

void state::update_person_destination(int n, int floor) {
	for(int i = 2; i >= 0; i--) {
		mystate[18+4*P+n*11+i] = floor%2;
		floor = floor/2;
	}
}

