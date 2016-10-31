#include "state.h"
#include <iostream>
#include <bitset>

using namespace std;

// N - number of floors
// K - number of elevators
// p - probabilty to arrive
// q - probabilty arrives at floor 1
// r - probabilty destination is floor 1

static int N, K, p, q, r;

// Used in generating next possible outputs from a given state
static double comb_count1 = 0, comb_count2 = 0;
vector<int> buttons1;
vector<int> combination1;
vector<vector<int>> all_combinations1;
vector<int> buttons2;
vector<int> combination2;
vector<vector<int>> all_combinations2;

// Create a class of states
class node {
	// state space
	state mystate;
	// Link to states
	vector<node> links;

	node(node new_state) {
		mystate.elevator = new_state.elevator;
		mystate.e0_dist = new_state.e0_dist;
		mystate.e1_dist = new_state.e1_dist;
		for(int i = 0; i < P; i++) {
			mystate.floor_dist[i] = new_state.floor_dist[i];
		}
		mystate.normalise = new_state.normalise;
	}
}initial_state, current_state;

// CREATE NEW STATES
void create_state(vector<string> actions, node current_state) {
	node new_state(current_state);

	for(int i = 0; i < actions.size(); i++) {
		// A PERSON BEING ADDED
		if(action[i][0] == 'P') {
			if(action[i][1] == 'I') {
				int person = (int)action[i][2] -'0';
				int floor = (int)action[i][3] - '0';
				new_state.add_person
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
node perform_input_action(vector<string> actions, node current_node) {
	node midway_node(current_node);
	vector<int> floor_buttons;
	int buttons_pressed = actions.size()-1;
	if(midway_node.mystate.isOpen(0)) {
		int floor = midway_node.mystate.getFloor(0);
		int direction = midway_node.mystate.getDirection(0)%2;
		midway_node.mystate.turnOffFloorButton(floor, direction);
		float floor_prob;
		vector<int> elevator_buttons;
		if(direction) {
			floor_prob = midway_node.mystate.getProbFloUp(floor);
			midway_node.mystate.setProbFloUp(floor, 0.0);
			for(int i = floor; i < N; i++) {
				if(midway_node.mystate.elevatorButtonPressed(0, i))
					elevator_buttons.push_back(i);
			}
		}
		else {
			floor_prob = midway_node.mystate.getProbFloDown(floor);
			midway_node.mystate.setProbFloDown(floor, 0.0);
			for(int i = 0; i < floor; i++) {
				if(midway_node.mystate.elevatorButtonPressed(0, i))
					elevator_buttons.push_back(i);
			}
		}
		int size = elevator_buttons.size();
		if(buttons_pressed == 0) {
			for(int i = 0; i < size; i++) {
				float elev_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 0);
				float prob = elev_prob + floor_prob / size;
				midway_node.mystate.setProbElev(0, elevator_buttons[i], prob);
			}
		}
		// else {
		// 	if(floor_prob > buttons_pressed) {
		// 		floor_prob = floor_prob - buttons_pressed;
		// 		for(int i = 0; i < buttons_pressed; i++) {
		// 		}
		// 	}
		// }
	}
	if(midway_node.mystate.isOpen(1)) {
		int floor = midway_node.mystate.getFloor(1);
		int direction = midway_node.mystate.getDirection(1)%2;
		midway_node.mystate.turnOffFloorButton(floor, direction);
		if(buttons_pressed == 0) {
			float floor_prob;
			vector<int> elevator_buttons;
			if(direction) {
				floor_prob = midway_node.mystate.getProbFloUp(floor);
				for(int i = floor; i < N; i++) {
					if(midway_node.mystate.elevatorButtonPressed(0, i))
						elevator_buttons.push_back(i);
				}
			}
			else {
				floor_prob = midway_node.mystate.getProbFloDown(floor);
				for(int i = 0; i < floor; i++) {
					if(midway_node.mystate.elevatorButtonPressed(0, i))
						elevator_buttons.push_back(i);
				}
			}
			int size = elevator_buttons.size();
			for(int i = 0; i < size; i++) {
				float elev_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 0);
				float prob = elev_prob + floor_prob / size;
				midway_node.mystate.setProbElev(0, elevator_buttons[i], prob);
			}
		}
	}
	if(action[0][0] == '0') {
		bool flag = false;
		for(int j = 0; j < N; j++) {
			if(midway_node.mystate.floor_button_pressed(0, 0) && j != 0)
				floor_buttons.push_back(2*j-1);
			if(midway_node.mystate.floor_button_pressed(0, 1) && j != N) {
				floor_buttons.push_back(2*j);
				if(j == 0)
					flag = true;
			}
		}
		int size = floor_buttons.size();
		for(int j = 0; j < size; j++) {
			if(floor_buttons[j]%2 == 0) {
				float prob = midway_node.mystate.getProbFloUp(j/2), new_prob;
				if(flag) {
					if(j == 0)
						new_prob = prob + p*q;
					else
						new_prob = prob + p*(1-q)/(size-1);
				}
				else
					new_prob = prob + p/size;
				midway_node.mystate.setProbFloUp(j/2, new_prob);
			}
			else {
				float prob = midway_node.mystate.getProbFloDown(j/2 + 1), new_prob;
				if(flag)
					new_prob = prob + p*(1-q)/(size-1);
				else 
					new_prob = prob + p/size;
				midway_node.mystate.setProbFloDown(j/2 + 1, new_prob);
			}
		}
	}
	else if(action[0][0] == 'B') {
		if(action[0][1] == 'U') {
			int floor = action[0][3] - '0';
			midway_node.mystate.turnOnFloorButton(floor, 1);
			midway_node.mystate.setProbFloUp(floor, 1.0);
		}
		else if(action[0][1] == 'D') {
			int floor = action[0][3] - '0';
			midway_node.mystate.turnOnFloorButton(floor, 0);
			midway_node.mystate.setProbFloDown(floor, 1.0);
		}	
	}
	return midway_node;
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

// // FUNCTIONS TO UPDATE THE STATE OF VARIABLES
// void update_floor(int n, int k, bitset<encoding_size> mystate) {
// 	for(int i = 2; i >= 0; i--) {
// 		mystate[-3+k*3+i] = n%2;
// 		n = n/2;
// 	}
// }

// void elevator_moves(int dir, int k, bitset<encoding_size> mystate) {
// 	bool down = false, up = false;
// 	int index = 4 + 2*k;
// 	if(dir == 1)
// 		up = true;
// 	else if(dir == -1)
// 		down = true;
// 	mystate[index] = down;
// 	mystate[index+1] = up;
// }

// void elevator_doors(bool open, int k, bitset<encoding_size> mystate) {
// 	if(open)
// 		mystate[9+k] = true;
// 	else
// 		mystate[9+k] = false;
// }

// void lift_button(int n, int k, bool pressed, bitset<encoding_size> mystate) {
// 	if(pressed) 
// 		mystate[6+P*k+n] = true;
// 	else
// 		mystate[6+P*k+n] = false;
// }

// void floor_button(int n, bool pressed, bitset<encoding_size> mystate) {
// 	if(pressed) 
// 		mystate[11+2*P+n] = true;
// 	else
// 		mystate[11+2*P+n] = false;
// }

// void add_person(int n, int floor, bitset<encoding_size> mystate) {
// 	mystate[10+4*P+n*11] = true;
// 	for(int i = 0; i < 4; i++) {
// 		mystate[11+4*P+n*11+i] = false;
// 	}
// 	for(int i = 2; i >= 0; i--) {
// 		mystate[15+4*P+n*11+i] = floor%2;
// 		floor = floor/2;
// 	}
// }

// void delete_person(int n, bitset<encoding_size> mystate) {
// 	for(int i = 0; i < 11; i++) {
// 		mystate[10+4*P+n*11+i] = false;
// 	}
// }

// void person_moves(int n, int loc, bitset<encoding_size> mystate) {
// 	for(int i = 2; i >= 0; i--) {
// 		mystate[15+4*P+n*11+i] = loc%2;
// 		loc = loc/2;
// 	}
// }

// void update_person_time(int n, bitset<encoding_size> mystate) {
// 	int time = get_waiting_time(n);
// 	time++;
// 	for(int i = 3; i >= 0; i--) {
// 		mystate[11+4*P+n*11+i] = time%2;
// 		time = time/2;
// 	}
// }

// void update_person_destination(int n, int floor, bitset<encoding_size> mystate) {
// 	for(int i = 2; i >= 0; i--) {
// 		mystate[18+4*P+n*11+i] = floor%2;
// 		floor = floor/2;
// 	}
// }

// // FUNCTIONS TO GET STATE OF VARIABLES
// bool any_lift_buttons_pressed(int k, bitset<encoding_size> mystate) {
// 	for(int i = 1; i <= P; i++) {
// 		if(mystate[6+k*P+i])
// 			return true;
// 	}
// 	return false;
// }

// bool any_lift_buttons_pressed_above(int n, int k, bitset<encoding_size> mystate) {
// 	for(int i = n+1; i <= P; i++) {
// 		if(mystate[6+k*P+i])
// 			return true;
// 	}
// 	return false;
// }

// bool any_lift_buttons_pressed_below(int n, int k, bitset<encoding_size> mystate) {
// 	for(int i = 1; i <= n-1; i++) {
// 		if(mystate[6+k*P+i])
// 			return true;
// 	}
// 	return false;
// }

// int get_destination(int n, bitset<encoding_size> mystate) {
// 	int destination;
// 	destination = mystate[18+4*P+n*11] + 2*mystate[19+4*P+n*11] + 4*mystate[20+4*P+n*11];
// 	return destination;
// }

// int get_location(int n, bitset<encoding_size> mystate) {
// 	int location;
// 	location = mystate[15+4*P+n*11] + 2*mystate[16+4*P+n*11] + 4*mystate[17+4*P+n*11];
// 	return location;
// }

// int get_waiting_time(int n, bitset<encoding_size> mystate) {
// 	int time;
// 	time = mystate[11+4*P+n*11] + 2*mystate[12+4*P+n*11] + 4*mystate[13+4*P+n*11] + 8*mystate[14+4*P+n*11];
// 	return time;
// }

// bool does_person_exist(int n, bitset<encoding_size> mystate) {
// 	if(mystate[10+4*P+n*11]) 
// 		return true;
// 	else
// 		return false;
// }

// bool floor_button_pressed(int n, bitset<encoding_size> mystate) {
// 	if(mystate[n+11+2*P])
// 		return true;
// 	else
// 		return false;
// }

// bool elevator_button_pressed(int n, int k, bitset<encoding_size> mystate) {
// 	if(k == 1) {
// 		if(mystate[11+n])
// 			return true;
// 		else
// 			return false;
// 	}
// 	else {
// 		if(mystate[11+P+n])
// 			return true;
// 		else
// 			return false;
// 	}
// }

// bool doors_opened(int k, bitset<encoding_size> mystate) {
// 	if(k == 1) {
// 		if(mystate[10]) 
// 			return true;
// 		else
// 			return false;
// 	}
// 	else {
// 		if(mystate[11]) 
// 			return true;
// 		else
// 			return false;
// 	}
// }

// int get_direction(int k, bitset<encoding_size> mystate) {
// 	int direction;
// 	if(k == 1) {
// 		if(mystate[6])
// 			direction = -1;
// 		else if(mystate[7])
// 			direction = 1;
// 		else
// 			direction = 0;
// 	}
// 	else {
// 		if(mystate[8])
// 			direction = -1;
// 		else if(mystate[9])
// 			direction = 1;
// 		else
// 			direction = 0;
// 	}
// 	return direction;
// }

// int get_floor(int k, bitset<encoding_size> mystate) {
// 	int floor;
// 	if(k == 1)
// 		floor = mystate[0] + mystate[1]*2 + mystate[2]*4;
// 	else
// 		floor = mystate[3] + mystate[4]*2 + mystate[5]*4;
// 	return floor;
// }

void go1(int offset, int k) {
  if (k == 0) {
    all_combinations1[comb_count1].push_back(combination1);
    comb_count1++;
    return;
  }
  for (int i = offset; i <= buttons1.size() - k; ++i) {
    combination1.push_back(buttons1[i]);
    go(i+1, k-1);
    combination1.pop_back();
  }
}

void go2(int offset, int k) {
  if (k == 0) {
    all_combinations2[comb_count2].push_back(combination2);
    comb_count2++;
    return;
  }
  for (int i = offset; i <= buttons2.size() - k; ++i) {
    combination2.push_back(buttons2[i]);
    go(i+1, k-1);
    combination2.pop_back();
  }
}

void generate_input_actions(node curr_node, vector<vector<string>> actions) {
	int flag1 = 0, flag2 = 0, count = 0;
	if(curr_node.mystate.isOpen(0)) {
		flag1 = 1;
	} 
	if(curr_node.mystate.isOpen(1)) {
		flag2 = 1;
	} 
	for(int i = 0; i <= 2*N - 2; i++) {
		vector<string> input_action;
		if(i == 0) {
			string myaction = "0";
			input_action.push_back(myaction);
		}
		else if (i != 0) {
			if(curr_node.mystate.floorButtonPressed(i))
				continue;
			else {
				string myaction;
				if(i%2 == 0) {
					myaction = "BD";
					myaction += i/2 + 1;
				}
				else {
					myaction = "BU";
					myaction += i/2 + 1;
				}
				input_action.push_back(myaction);
			}
		}
		int direction = curr_node.mystate.getDirection(0);
		float prob;
		if(direction == 1)
			prob = curr_node.mystate.getProbFloUp(curr_node.mystate.getFloor(0));
		else if(direction == -1)
			prob = curr_node.mystate.getProbFloDown(curr_node.mystate.getFloor(0));
		if(flag1 == 1) {
			buttons1.clear();
			for(int j = 0; j < N; j++) {
				if(!curr_node.mystate.elevatorButtonPressed(0,j) && ((curr_node.mystate.getDirection(0)%2 == 1 && j >= curr_node.mystate.getFloor(0)) || (curr_node.mystate.getDirection(0)%2 == 0 && j-1 < curr_node.mystate.getFloor(0)))) {
					buttons1.push_back(j);
				}
			}
			comb_count1 = 0;
			for(int j = 1; j <= ceil(prob); j++) {
				combination1.clear();
				go1(0,j);
			}
			for(int j = 0; j < comb_count1; j++) {
				vector<string> next_action(input_action);
				for(int k = 0; k < all_combinations1[j].size(); k++) {
					string new_action = "B_";
					new_action += all_combinations1[j][k];
					new_action += "_1";
					next_action.push_back(new_action);
				}
				if(flag2 == 0)
					actions.push_back(next_action);
				else {
					buttons2.clear();
					for(int j = 0; j < N; j++) {
						if(!curr_node.mystate.elevatorButtonPressed(1,j) && ((curr_node.mystate.getDirection(1)%2 == 1 && j >= curr_node.mystate.getFloor(1)) || (curr_node.mystate.getDirection(1)%2 == 0 && j-1 < curr_node.mystate.getFloor(1)))) {
							buttons2.push_back(j);
						}
					}
					comb_count2 = 0;
					for(int j = 1; j <= ceil(prob); j++) {
						combination2.clear();
						go2(0,j);
					}
					for(int j = 0; j < comb_count2; j++) {
						vector<string> newer_action(next_action);
						for(int k = 0; k < all_combinations2[j].size(); k++) {
							string new_action = "B_";
							new_action += all_combinations2[j][k];
							new_action += "_1";
							newer_action.push_back(new_action);
						}
						actions.push_back(newer_action);
					}
				}
			}
		}
		else if(flag2 == 1) {
			buttons2.clear();
			for(int j = 0; j < N; j++) {
				if(!curr_node.mystate.elevatorButtonPressed(1,j) && ((curr_node.mystate.getDirection(1)%2 == 1 && j >= curr_node.mystate.getFloor(1)) || (curr_node.mystate.getDirection(1)%2 == 0 && j-1 < curr_node.mystate.getFloor(1)))) {
					buttons2.push_back(j);
				}
			}
			comb_count2 = 0;
			for(int j = 1; j <= ceil(prob); j++) {
				combination2.clear();
				go2(0,j);
			}
			for(int j = 0; j < comb_count2; j++) {
				vector<string> next_action(input_action);
				for(int k = 0; k < all_combinations2[j].size(); k++) {
					string new_action = "B_";
					new_action += all_combinations2[j][k];
					new_action += "_1";
					next_action.push_back(new_action);
				}
				actions.push_back(next_action);
			}
		}
		else
			actions.push_back(input_action);
	}
}

void split(string input, vector<string> &input_actions) {
	// Split input string
		string delimiter = " ";
		size_t pos = 0;
		string token;
		while ((pos = input.find(delimiter)) != string::npos) {
		    token = input.substr(0, pos);
		    input_actions.push_back(token);
		    input.erase(0, pos + delimiter.length());
		}
}

int main(int argc, char* argv[]) {
	cout<<"Lets get started Ankush!";
	N = argv[1];
	K = argv[2];
	p = argv[3];
	q = argv[4];
	r = argv[5];
	string input;
	do {
		cin>>input;
		vector<string> input_actions;
		split(input, input_actions);
		for(int i = 0; i < input_actions.size(); i++) {
			if(input_actions[i] != "0")
				perform_input_action(input_actions[i]);
		}
	} while(input != "DONE");
	
	return 0;
}