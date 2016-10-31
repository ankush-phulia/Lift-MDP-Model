#include <iostream>
#include <bitset>

using namespace std;

// No of floors and the number of elevators 
int P, Q, R; // R  = log P rounded up
int encoding_size;
static double count = 0;

// Create a class of states
class state {
	// state space
	std::bitset<encoding_size> mystate;
	// Link to states
	vector<state> links;

	state(state new_state) {
		for(int i = 0; i < encoding_size; i++) {
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
			else if(action[i][1] == 'O') {
				int button = (int)action[i][2] - '0';
				floor_button(button, false, new_state.mystate);
			}
			else if(action[i][1] == 'F') {
				int floor = (int)action[i][2] - '0';
				int elevator = (int)action[i][3] - '0';
				lift_button(floor, elevator, false, new_state.mystate);
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
	count++;
}

// APPLY BUTTON PUSHES
void perform_input_action(vector<string> actions, state current_state) {
	int extra_button_pressed = 0;
	vector<int> lift_buttons_pressed;
	int floors[P];

	for(int i = 0; i < P; i++) {
		floors[i] = 0;
	}

	for(int i = 0; i <actions.size(); i++) {
		if(action[i][0] == '0') {
			for(int j = 1; j <= 2*P-2; j++) {
				if(floor_button_pressed(j)) {
					int curr_floor = j/2;
					floors[curr_floor] = 1;
				}
			}
		}
		else if(action[i][0] == 'B') {
			if(action[i][1] == 'U') {
				int floor = (int)action[i][2] - '0';
				new_actions.push_back(action[i]);
				string add_person_action= "PI";
				for(int i = 0; i < no_of_people; i++) {
					if(!does_person_exist(i)) {
						string += i;
						break;
					}
				}
				new_actions.push_back(add_person_action);
				extra_button_pressed = 2*floor - 1;
			}
			else if(action[i][1] == 'D') {
				int floor = (int)action[i][2] - '0';
				new_actions.push_back(action[i]);
				string add_person_action= "PI";
				for(int i = 0; i < no_of_people; i++) {
					if(!does_person_exist(i)) {
						string += i;
						break;
					}
				}
				new_actions.push_back(add_person_action);
				extra_button_pressed = 2*(floor-1);
			}	
			else {
				int floor = (int)action[i][1] - '0';
				int elevator = (int)action[i][2] - '0';
				int button_value = floor + P*elevator;
				lift_buttons_pressed.push_back(button_value);
				new_actions.push_back(action[i]);
			}
		}
	}
}

// PERFORM ACTION
void perform_output_action(vector<string> actions, vector<string> new_actions, state current_state) {

	// Lets get some variables from the current state
	int new_floor1 = get_floor(1, current_state.mystate);
	int new_floor2 = get_floor(2, current_state.mystate);
	int direction1 = get_direction(1, current_state.mystate);
	int direction2 = get_direction(2, current_state.mystate);
	
	int floor_button1 = 2*(new_floor1-1);
	int floor_button2 = 2*(new_floor1-1)+1;
	if(floor_button2 == 9)
		floor_button2 = 0;
	int floor_button3 = 2*(new_floor2-1);
	int floor_button4 = 2*(new_floor2-1)+1;
	if(floor_button4 == 9)
		floor_button4 = 0;
	if(floor_button_pressed(floor_button1) && direction1 == -1) {
		string open_doors = "AOD1";
		new_actions.push_back(open_doors);
		string turn_off_button = "BO";
		turn_off_button += floor_button1;
		new_actions.push_back(turn_off_button);
		string turn_off_lift_button = "BF";
		turn_off_button += new_floor1;
		turn_off_button += '1';
		new_actions.push_back(turn_off_lift_button);
		for(int i = 0; i < no_of_people; i++) {
			if(get_location(i) == new_floor1) {
				string change_location = "PL";
				change_location += i;
				change_location += P + 1;
				new_actions.push_back(change_location);
			}
			if(get_destination(i) == new_floor1) {
				string remove_person = "PO";
				remove_person += i;
				new_actions.push_back(remove_person);
			}
		}
	}
	else if(floor_button_pressed(floor_button2) && direction1 == 1) {
		string open_doors = "AOU1";
		new_actions.push_back(open_doors);
		string turn_off_button = "BO";
		turn_off_button += floor_button1;
		new_actions.push_back(turn_off_button);
		string turn_off_lift_button = "BF";
		turn_off_button += new_floor1;
		turn_off_button += '1';
		new_actions.push_back(turn_off_lift_button);
		for(int i = 0; i < no_of_people; i++) {
			if(get_location(i) == new_floor1) {
				string change_location = "PL";
				change_location += i;
				change_location += P + 1;
				new_actions.push_back(change_location);
			}
			if(get_destination(i) == new_floor1) {
				string remove_person = "PO";
				remove_person += i;
				new_actions.push_back(remove_person);
			}
		}
	}
	else if(doors_opened(1)) {
		string doors_closed = "AC"
		if(direction1 == 1)
			doors_closed += 'U';
		else if(direction1 == -1)
			doors_closed += 'D';
		doors_closed += new_floor1;
		new_actions.push_back(doors_closed);
		if(direction1 == 1)
			new_floor1++;
		else if(direction1 == -1)
			new_floor1--;
		string update_floor = "A1";
		update_floor += new_floor1;
		new_actions.push_back(update_floor);
	}
	else if(direction1 == 1 && any_lift_buttons_pressed_above(new_floor1)) {
		new_floor1++;
		string update_floor = "A1";
		update_floor += new_floor1;
		new_actions.push_back(update_floor);
		string update_direction = "AU1";
		new_actions.push_back(update_direction);
	}
	
	if(floor_button_pressed(floor_button3) && direction2 == -1) {
		string open_doors = "AOD2";
		new_actions.push_back(open_doors);
		string turn_off_button = "BO";
		turn_off_button += floor_button1;
		new_actions.push_back(turn_off_button);
		string turn_off_lift_button = "BF";
		turn_off_button += new_floor2;
		turn_off_button += '2';
		new_actions.push_back(turn_off_lift_button);
		for(int i = 0; i < no_of_people; i++) {
			if(get_location(i) == new_floor2) {
				string change_location = "PL";
				change_location += i;
				change_location += P + 2;
				new_actions.push_back(change_location);
			}
			if(get_destination(i) == new_floor2) {
				string remove_person = "PO";
				remove_person += i;
				new_actions.push_back(remove_person);
			}
		}
	}
	else if(floor_button_pressed(floor_button3) && direction2 == 1) {
		string open_doors = "AOU2";
		new_actions.push_back(open_doors);
		string turn_off_button = "BO";
		turn_off_button += floor_button1;
		new_actions.push_back(turn_off_button);
		string turn_off_lift_button = "BF";
		turn_off_button += new_floor2;
		turn_off_button += '2';
		new_actions.push_back(turn_off_lift_button);
		for(int i = 0; i < no_of_people; i++) {
			if(get_location(i) == new_floor2) {
				string change_location = "PL";
				change_location += i;
				change_location += P + 2;
				new_actions.push_back(change_location);
			}
			if(get_destination(i) == new_floor2) {
				string remove_person = "PO";
				remove_person += i;
				new_actions.push_back(remove_person);
			}
		}
	}
	else if(doors_opened(2)) {
		string doors_closed = "AC"
		if(direction2 == 1)
			doors_closed += 'U';
		else if(direction2 == -1)
			doors_closed += 'D';
		doors_closed += new_floor2;
		new_actions.push_back(doors_closed);
		if(direction2 == 1)
			new_floor2++;
		else if(direction2 == -1)
			new_floor2--;
		string update_floor = "A2";
		update_floor += new_floor2;
		new_actions.push_back(update_floor);
	}
}

// FUNCTIONS TO UPDATE THE STATE OF VARIABLES
void update_floor(int n, int k, bitset<encoding_size> mystate) {
	for(int i = 2; i >= 0; i--) {
		mystate[-3+k*3+i] = n%2;
		n = n/2;
	}
}

void elevator_moves(int dir, int k, bitset<encoding_size> mystate) {
	bool down = false, up = false;
	int index = 4 + 2*k;
	if(dir == 1)
		up = true;
	else if(dir == -1)
		down = true;
	mystate[index] = down;
	mystate[index+1] = up;
}

void elevator_doors(bool open, int k, bitset<encoding_size> mystate) {
	if(open)
		mystate[9+k] = true;
	else
		mystate[9+k] = false;
}

void lift_button(int n, int k, bool pressed, bitset<encoding_size> mystate) {
	if(pressed) 
		mystate[6+P*k+n] = true;
	else
		mystate[6+P*k+n] = false;
}

void floor_button(int n, bool pressed, bitset<encoding_size> mystate) {
	if(pressed) 
		mystate[11+2*P+n] = true;
	else
		mystate[11+2*P+n] = false;
}

void add_person(int n, int floor, bitset<encoding_size> mystate) {
	mystate[10+4*P+n*11] = true;
	for(int i = 0; i < 4; i++) {
		mystate[11+4*P+n*11+i] = false;
	}
	for(int i = 2; i >= 0; i--) {
		mystate[15+4*P+n*11+i] = floor%2;
		floor = floor/2;
	}
}

void delete_person(int n, bitset<encoding_size> mystate) {
	for(int i = 0; i < 11; i++) {
		mystate[10+4*P+n*11+i] = false;
	}
}

void person_moves(int n, int loc, bitset<encoding_size> mystate) {
	for(int i = 2; i >= 0; i--) {
		mystate[15+4*P+n*11+i] = loc%2;
		loc = loc/2;
	}
}

void update_person_time(int n, bitset<encoding_size> mystate) {
	int time = get_waiting_time(n);
	time++;
	for(int i = 3; i >= 0; i--) {
		mystate[11+4*P+n*11+i] = time%2;
		time = time/2;
	}
}

void update_person_destination(int n, int floor, bitset<encoding_size> mystate) {
	for(int i = 2; i >= 0; i--) {
		mystate[18+4*P+n*11+i] = floor%2;
		floor = floor/2;
	}
}

// FUNCTIONS TO GET STATE OF VARIABLES
bool any_lift_buttons_pressed(int k, bitset<encoding_size> mystate) {
	for(int i = 1; i <= P; i++) {
		if(mystate[6+k*P+i])
			return true;
	}
	return false;
}

bool any_lift_buttons_pressed_above(int n, int k, bitset<encoding_size> mystate) {
	for(int i = n+1; i <= P; i++) {
		if(mystate[6+k*P+i])
			return true;
	}
	return false;
}

bool any_lift_buttons_pressed_below(int n, int k, bitset<encoding_size> mystate) {
	for(int i = 1; i <= n-1; i++) {
		if(mystate[6+k*P+i])
			return true;
	}
	return false;
}

int get_destination(int n, bitset<encoding_size> mystate) {
	int destination;
	destination = mystate[18+4*P+n*11] + 2*mystate[19+4*P+n*11] + 4*mystate[20+4*P+n*11];
	return destination;
}

int get_location(int n, bitset<encoding_size> mystate) {
	int location;
	location = mystate[15+4*P+n*11] + 2*mystate[16+4*P+n*11] + 4*mystate[17+4*P+n*11];
	return location;
}

int get_waiting_time(int n, bitset<encoding_size> mystate) {
	int time;
	time = mystate[11+4*P+n*11] + 2*mystate[12+4*P+n*11] + 4*mystate[13+4*P+n*11] + 8*mystate[14+4*P+n*11];
	return time;
}

bool does_person_exist(int n, bitset<encoding_size> mystate) {
	if(mystate[10+4*P+n*11]) 
		return true;
	else
		return false;
}

bool floor_button_pressed(int n, bitset<encoding_size> mystate) {
	if(mystate[n+11+2*P])
		return true;
	else
		return false;
}

bool elevator_button_pressed(int n, int k, bitset<encoding_size> mystate) {
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

bool doors_opened(int k, bitset<encoding_size> mystate) {
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

int get_direction(int k, bitset<encoding_size> mystate) {
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

int get_floor(int k, bitset<encoding_size> mystate) {
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
	encoding_size = 200;
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