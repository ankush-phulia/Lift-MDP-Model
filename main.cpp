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
	vector<*node> links;

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

// APPLY BUTTON PUSHES
node perform_input_action(vector<string> actions, node current_node) {
	node midway_node(current_node);
	vector<int> floor_buttons;
	vector<string> elevator1_button_actions;
	vector<string> elevator2_button_actions;
	int buttons_pressed1 = 0;
	int buttons_pressed2 = 0;
	int floor1_elev1_flag = 0;
	int floor1_elev2_flag = 0;
	for(int i = 1; i < actions.size(); i++) {
		if(actions[i][4] == '1') {
			if(actions[i][2] == '1')
				floor1_elev1_flag = 1;
			elevator1_button_actions.push_back(actions[i]);
			buttons_pressed1++;
		}
		else {
			if(actions[i][2] == '1')
				floor1_elev2_flag = 1;
			elevator2_button_actions.push_back(actions[i]);
			buttons_pressed2++;
		}
	}

	if(midway_node.mystate.isOpen(1)) {
		int floor = midway_node.mystate.getFloor(1);
		int direction = midway_node.mystate.getDirection(1)%3;
		midway_node.mystate.turnOffFloorButton(floor, direction);
		float floor_prob;
		vector<int> elevator_buttons;
		if(direction) {
			floor_prob = midway_node.mystate.getProbFloUp(floor);
			midway_node.mystate.setProbFloUp(floor, 0.0);
			for(int i = floor+1; i <= N; i++) {
				if(midway_node.mystate.elevatorButtonPressed(1, i))
					elevator_buttons.push_back(i);
			}
		}
		else {
			floor_prob = midway_node.mystate.getProbFloDown(floor);
			midway_node.mystate.setProbFloDown(floor, 0.0);
			for(int i = 1; i < floor; i++) {
				if(midway_node.mystate.elevatorButtonPressed(1, i))
					elevator_buttons.push_back(i);
				if(i == 1) 
					floor1_elev1_flag = 2;
			}
		}
		int size = elevator_buttons.size();
		if(buttons_pressed1 == 0) {
			for(int i = 0; i < size; i++) {
				float elev_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 0);
				float prob = elev_prob + floor_prob / size;
				midway_node.mystate.setProbElev(1, elevator_buttons[i], prob);
			}
		}
		else {
			if(floor_prob > buttons_pressed) {
				int total_buttons_pressed = buttons_pressed1 + elevator_buttons.size();
				floor_prob = floor_prob - buttons_pressed1;
				for(int i = 0; i < buttons_pressed1; i++) {
					int floor_no = elevator1_button_actions[i][3];
					float elev_floor_prob = midway_node.mystate.getProbElev(floor_no, 1);
					if(floor_no == 1)
						elev_floor_prob += (1.0 + floor_prob*r);
					if(floor1_elev1_flag)
						elev_floor_prob += (1.0 + floor_prob*(1-r)/(total_buttons_pressed-1));
					midway_node.mystate.setProbElev(1, floor_no, elev_floor_prob);
				}
				for(int i = 0; i < elevator_buttons.size(); i++) {
					float elev_floor_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 1);
					if(floor_no == 1)
						elev_floor_prob += (floor_prob*r);
					if(floor1_elev1_flag)
						elev_floor_prob += (floor_prob*(1-r)/(total_buttons_pressed-1));
					midway_node.mystate.setProbElev(1, floor_no, elev_floor_prob);
				}
			}
			else {
				for(int i = 0; i < buttons_pressed1; i++) {
					int floor_no = elevator1_button_actions[i][3];
					float elev_floor_prob = midway_node.mystate.getProbElev(floor_no, 1);
					elev_floor_prob += (1.0);
					midway_node.mystate.setProbElev(1, floor_no, elev_floor_prob);
				}
			}
		}
	}

	if(midway_node.mystate.isOpen(2)) {
		int floor = midway_node.mystate.getFloor(2);
		int direction = midway_node.mystate.getDirection(2)%3;
		midway_node.mystate.turnOffFloorButton(floor, direction);
		float floor_prob;
		vector<int> elevator_buttons;
		if(direction) {
			floor_prob = midway_node.mystate.getProbFloUp(floor);
			midway_node.mystate.setProbFloUp(floor, 0.0);
			for(int i = floor+1; i <= N; i++) {
				if(midway_node.mystate.elevatorButtonPressed(2, i))
					elevator_buttons.push_back(i);
			}
		}
		else {
			floor_prob = midway_node.mystate.getProbFloDown(floor);
			midway_node.mystate.setProbFloDown(floor, 0.0);
			for(int i = 1; i < floor; i++) {
				if(midway_node.mystate.elevatorButtonPressed(2, i))
					elevator_buttons.push_back(i);
				if(i == 1) 
					floor1_elev2_flag = 2;
			}
		}
		int size = elevator_buttons.size();
		if(buttons_pressed2 == 0) {
			for(int i = 0; i < size; i++) {
				float elev_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 0);
				float prob = elev_prob + floor_prob / size;
				midway_node.mystate.setProbElev(2, elevator_buttons[i], prob);
			}
		}
		else {
			if(floor_prob > buttons_pressed) {
				int total_buttons_pressed = buttons_pressed2 + elevator_buttons.size();
				floor_prob = floor_prob - buttons_pressed2;
				for(int i = 0; i < buttons_pressed2; i++) {
					int floor_no = elevator2_button_actions[i][3];
					float elev_floor_prob = midway_node.mystate.getProbElev(floor_no, 2);
					if(floor_no == 1)
						elev_floor_prob += (1.0 + floor_prob*r);
					if(floor1_elev2_flag)
						elev_floor_prob += (1.0 + floor_prob*(1-r)/(total_buttons_pressed-1));
					midway_node.mystate.setProbElev(2, floor_no, elev_floor_prob);
				}
				for(int i = 0; i < elevator_buttons.size(); i++) {
					float elev_floor_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 2);
					if(floor_no == 1)
						elev_floor_prob += (floor_prob*r);
					if(floor1_elev2_flag)
						elev_floor_prob += (floor_prob*(1-r)/(total_buttons_pressed-1));
					midway_node.mystate.setProbElev(2, floor_no, elev_floor_prob);
				}
			}
			else {
				for(int i = 0; i < buttons_pressed1; i++) {
					int floor_no = elevator2_button_actions[i][3];
					float elev_floor_prob = midway_node.mystate.getProbElev(floor_no, 1);
					elev_floor_prob += (1.0);
					midway_node.mystate.setProbElev(2, floor_no, elev_floor_prob);
				}
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

// Action enocding
/* 
	FIRST 4 bits Elevator 1
	SECOND 4 bits Elevator 2
	0010 - UP - 0
	0100 - DOWN - 1
	1000 - STILL - 2
	0000 - OPENUP - 3
	0001 - OPENDOWN - 4
*/ 
void get_action1(vector<int> &actions, node midway_node) {
// Lets get some variables from the current state
	int floor1 = midway_node.mystate.getFloor(1);
	int direction1 = midway_node.mystate.getDirection(1)%3;

	if(midway_node.mystate.elevatorButtonPressed(1, floor1)) {
		if(direction1) {
			bool pressed = false;
			for(int i = floor1 + 1; i <= N; i++) {
				if(midway_node.mystate.elevatorButtonPressed(1, i)) {
					pressed = true;
					break;
				}
			}
			if(pressed) {
				actions.push_back(3);
			}
			else {
				if(midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)
						actions.push_back(3);
				}
				else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
						actions.push_back(4);
				}	
				else{
					if(floor1 != 5)
						actions.push_back(3);
					if(floor1 != 1)
						actions.push_back(4);
				}
			}
		}
		if(!direction1) {
			bool pressed = false;
			for(int i = 0; i < floor1; i++) {
				if(midway_node.mystate.elevatorButtonPressed(1, i)) {
					pressed = true;
					break;
				}
			}
			if(pressed) {
				actions.push_back(4);
			}
			else {
				if(midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)
						actions.push_back(3);
				}
				else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
						actions.push_back(4);
				}	
				else{
					if(floor1 != 5)
						actions.push_back(3);
					if(floor1 != 1)
						actions.push_back(4);
				}
			}
		}
	}
	else {
		bool pressed_up = false;
		bool pressed_down = false;
		for(int i = floor1 + 1; i <= N; i++) {
			if(midway_node.mystate.elevatorButtonPressed(1, i)) {
				pressed_up = true;
				break;
			}
		}
		for(int i = 1; i < floor1; i++) {
			if(midway_node.mystate.elevatorButtonPressed(1, i)) {
				pressed_down = true;
				break;
			}
		}
		if(pressed_up) {
			if(midway_node.mystate.floorButtonPressed(floor1, 1)) {
				actions.push_back(3);
			}
			else {
				actions.push_back(0);
			}
		}
		if(pressed_down) {
			if(midway_node.mystate.floorButtonPressed(floor1, 0)) {
				actions.push_back(4);
			}
			else {
				actions.push_back(1);
			}
		}
		else {
			if(midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5) {
			 	if(!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)
					actions.push_back(3);
			}
			else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
			 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
					actions.push_back(4);
			}
			else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5)) {
				actions.push_back(3);
				actions.push_back(4);
			}
			else {
				actions.push_back(2);
				bool any_floor_button_pressed_below = false;
				bool any_floor_button_pressed_above = false;
				for(int i = 1; i <= N; i++) {
					if(midway_node.mystate.floorButtonPressed(i, 1) && i != 5) {
						if(i > floor1)
							any_floor_button_pressed_above = true;
						else
							any_floor_button_pressed_below = true;
					}
					if(midway_node.mystate.floorButtonPressed(i, 0) && i != 1) {
						if(i > floor1)
							any_floor_button_pressed_above = true;
						else
							any_floor_button_pressed_below = true;
					}
					if(any_floor_button_pressed_above)
						actions.push_back(0);
					if(any_floor_button_pressed_below)
						actions.push_back(1);
				}
			}
		}
	}
}

void get_action2(vector<int> &actions, node midway_node) {
// Lets get some variables from the current state
	int floor1 = midway_node.mystate.getFloor(2);
	int direction1 = midway_node.mystate.getDirection(2)%3;

	if(midway_node.mystate.elevatorButtonPressed(2, floor1)) {
		if(direction1) {
			bool pressed = false;
			for(int i = floor1 + 1; i <= N; i++) {
				if(midway_node.mystate.elevatorButtonPressed(2, i)) {
					pressed = true;
					break;
				}
			}
			if(pressed) {
				actions.push_back(3);
			}
			else {
				if(midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)
						actions.push_back(3);
				}
				else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
						actions.push_back(4);
				}	
				else{
					if(floor1 != 5)
						actions.push_back(3);
					if(floor1 != 1)
						actions.push_back(4);
				}
			}
		}
		if(!direction1) {
			bool pressed = false;
			for(int i = 0; i < floor1; i++) {
				if(midway_node.mystate.elevatorButtonPressed(2, i)) {
					pressed = true;
					break;
				}
			}
			if(pressed) {
				actions.push_back(4);
			}
			else {
				if(midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)
						actions.push_back(3);
				}
				else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
						actions.push_back(4);
				}	
				else{
					if(floor1 != 5)
						actions.push_back(3);
					if(floor1 != 1)
						actions.push_back(4);
				}
			}
		}
	}
	else {
		bool pressed_up = false;
		bool pressed_down = false;
		for(int i = floor1 + 1; i <= N; i++) {
			if(midway_node.mystate.elevatorButtonPressed(2, i)) {
				pressed_up = true;
				break;
			}
		}
		for(int i = 1; i < floor1; i++) {
			if(midway_node.mystate.elevatorButtonPressed(2, i)) {
				pressed_down = true;
				break;
			}
		}
		if(pressed_up) {
			if(midway_node.mystate.floorButtonPressed(floor1, 1)) {
				actions.push_back(3);
			}
			else {
				actions.push_back(0);
			}
		}
		if(pressed_down) {
			if(midway_node.mystate.floorButtonPressed(floor1, 0)) {
				actions.push_back(4);
			}
			else {
				actions.push_back(1);
			}
		}
		else {
			if(midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5) {
			 	if(!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)
					actions.push_back(3);
			}
			else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
			 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
					actions.push_back(4);
			}
			else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5)) {
				actions.push_back(3);
				actions.push_back(4);
			}
			else {
				actions.push_back(2);
				bool any_floor_button_pressed_below = false;
				bool any_floor_button_pressed_above = false;
				for(int i = 1; i <= N; i++) {
					if(midway_node.mystate.floorButtonPressed(i, 1) && i != 5) {
						if(i > floor1)
							any_floor_button_pressed_above = true;
						else
							any_floor_button_pressed_below = true;
					}
					if(midway_node.mystate.floorButtonPressed(i, 0) && i != 1) {
						if(i > floor1)
							any_floor_button_pressed_above = true;
						else
							any_floor_button_pressed_below = true;
					}
					if(any_floor_button_pressed_above)
						actions.push_back(0);
					if(any_floor_button_pressed_below)
						actions.push_back(1);
				}
			}
		}
	}
}

void generate_actions(vector<uint8_t> &actions, node midway_node) {
	vector<int> action1;
	vector<int> action2;
	get_action1(action1, midway_node);
	get_action2(action2, midway_node);

	for(int i = 0; i < action1.size(); i++) {
		for(int j = 0; j < action2.size(); j++) {
			uint8_t encoded = getencoding(action1[i], action2[i]);
			actions.push_back(encoded);
		}
	}
}

// PERFORM ACTION
node perform_output_action(vector<uint8_t> actions, node midway_node) {
	
	node final_node(midway_node);

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

void generate_input_actions(node curr_node, vector<vector<string>> &actions) {
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
					myaction = "BD_";
					myaction += i/2 + 1;
				}
				else {
					myaction = "BU_";
					myaction += i/2 + 1;
				}
				input_action.push_back(myaction);
			}
		}
		int direction = curr_node.mystate.getDirection(0)%3;
		float prob;
		if(direction == 1)
			prob = curr_node.mystate.getProbFloUp(curr_node.mystate.getFloor(0));
		else if(direction == -1)
			prob = curr_node.mystate.getProbFloDown(curr_node.mystate.getFloor(0));
		if(flag1 == 1) {
			buttons1.clear();
			for(int j = 0; j < N; j++) {
				if(!curr_node.mystate.elevatorButtonPressed(0,j) && ((curr_node.mystate.getDirection(0)%3 == 1 && j >= curr_node.mystate.getFloor(0)) || (curr_node.mystate.getDirection(0)%3 == 0 && j-1 < curr_node.mystate.getFloor(0)))) {
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
						if(!curr_node.mystate.elevatorButtonPressed(1,j) && ((curr_node.mystate.getDirection(1)%3 == 1 && j >= curr_node.mystate.getFloor(1)) || (curr_node.mystate.getDirection(1)%3 == 0 && j-1 < curr_node.mystate.getFloor(1)))) {
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
				if(!curr_node.mystate.elevatorButtonPressed(1,j) && ((curr_node.mystate.getDirection(1)%3 == 1 && j >= curr_node.mystate.getFloor(1)) || (curr_node.mystate.getDirection(1)%3 == 0 && j-1 < curr_node.mystate.getFloor(1)))) {
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