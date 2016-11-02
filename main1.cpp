#include "state.h"
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <math.h>


using namespace std;

// N - number of floors
// K - number of elevators
// p - probabilty to arrive
// q - probabilty arrives at floor 1
// r - probabilty destination is floor 1

static int N, K, p, q, r, t;
const int WAIT_TIME_COST_FACTOR = 2;
const int UP_DOWN_COST_FACTOR = 1;
static float total_cost = 0; 
static int rollout = 10;

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
public:
	// state space
	state mystate;
	// Link to states
	// vector<*node> links;

	node(const node &new_state) {
		mystate.elevator = new_state.mystate.elevator;
		mystate.e1_dist = new_state.mystate.e1_dist;
		mystate.e2_dist = new_state.mystate.e2_dist;
		for(int i = 0; i < N; i++) {
			mystate.floor_dist[i] = new_state.mystate.floor_dist[i];
		}
		mystate.normalise = new_state.mystate.normalise;
	}

	node() {

	}
};

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
				float elev_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 1);
				float prob = elev_prob + floor_prob / size;
				midway_node.mystate.setProbElev(1, elevator_buttons[i], prob);
			}
		}
		else {
			if(floor_prob > buttons_pressed1) {
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
					if(elevator_buttons[i] == 1)
						elev_floor_prob += (floor_prob*r);
					if(floor1_elev1_flag)
						elev_floor_prob += (floor_prob*(1-r)/(total_buttons_pressed-1));
					midway_node.mystate.setProbElev(1, elevator_buttons[i], elev_floor_prob);
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
		midway_node.mystate.setProbElev(1, floor, 0.0);
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
				float elev_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 2);
				float prob = elev_prob + floor_prob / size;
				midway_node.mystate.setProbElev(2, elevator_buttons[i], prob);
			}
		}
		else {
			if(floor_prob > buttons_pressed2) {
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
					if(elevator_buttons[i] == 1)
						elev_floor_prob += (floor_prob*r);
					if(floor1_elev2_flag)
						elev_floor_prob += (floor_prob*(1-r)/(total_buttons_pressed-1));
					midway_node.mystate.setProbElev(2, elevator_buttons[i], elev_floor_prob);
				}
			}
			else {
				for(int i = 0; i < buttons_pressed1; i++) {
					int floor_no = elevator2_button_actions[i][3];
					float elev_floor_prob = midway_node.mystate.getProbElev(floor_no, 2);
					elev_floor_prob += (1.0);
					midway_node.mystate.setProbElev(2, floor_no, elev_floor_prob);
				}
			}
		}
		midway_node.mystate.setProbElev(2, floor, 0.0);
	}

	if(actions[0][0] == '0') {
		bool flag = false;
		for(int j = 0; j < N; j++) {
			if(midway_node.mystate.floorButtonPressed(0, 0) && j != 0)
				floor_buttons.push_back(2*j-1);
			if(midway_node.mystate.floorButtonPressed(0, 1) && j != N) {
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
	else if(actions[0][0] == 'B') {
		if(actions[0][1] == 'U') {
			int floor = actions[0][3] - '0';
			midway_node.mystate.turnOnFloorButton(floor, 1);
			midway_node.mystate.setProbFloUp(floor, 1.0);
		}
		else if(actions[0][1] == 'D') {
			int floor = actions[0][3] - '0';
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
				actions.push_back(4);
			}
			else {
				if(midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)
						actions.push_back(4);
				}
				else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
						actions.push_back(5);
				}	
				else{
					if(floor1 != 5)
						actions.push_back(4);
					if(floor1 != 1)
						actions.push_back(5);
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
						actions.push_back(4);
				}
				else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
						actions.push_back(5);
				}	
				else{
					if(floor1 != 5)
						actions.push_back(4);
					if(floor1 != 1)
						actions.push_back(5);
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
				actions.push_back(4);
			}
			else {
				actions.push_back(1);
			}
		}
		if(pressed_down) {
			if(midway_node.mystate.floorButtonPressed(floor1, 0)) {
				actions.push_back(5);
			}
			else {
				actions.push_back(2);
			}
		}
		else {
			if(midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5) {
			 	if(!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)
					actions.push_back(4);
			}
			else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
			 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
					actions.push_back(5);
			}
			else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5)) {
				actions.push_back(4);
				actions.push_back(5);
			}
			else {
				// actions.push_back(3);
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
				}
				if(any_floor_button_pressed_above)
					actions.push_back(1);
				else if(floor1 != 5)
					actions.push_back(5);
				if(any_floor_button_pressed_below)
					actions.push_back(2);
				else if(floor1 != 1)
					actions.push_back(4);
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
				actions.push_back(4);
			}
			else {
				if(midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)
						actions.push_back(4);
				}
				else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
						actions.push_back(5);
				}	
				else{
					if(floor1 != 5)
						actions.push_back(4);
					if(floor1 != 1)
						actions.push_back(5);
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
				actions.push_back(5);
			}
			else {
				if(midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)
						actions.push_back(4);
				}
				else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
				 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
						actions.push_back(5);
				}	
				else{
					if(floor1 != 5)
						actions.push_back(4);
					if(floor1 != 1)
						actions.push_back(5);
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
				actions.push_back(4);
			}
			else {
				actions.push_back(1);
			}
		}
		if(pressed_down) {
			if(midway_node.mystate.floorButtonPressed(floor1, 0)) {
				actions.push_back(5);
			}
			else {
				actions.push_back(2);
			}
		}
		else {
			if(midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5) {
			 	if(!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)
					actions.push_back(4);
			}
			else if(midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) {
			 	if(!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == 5)
					actions.push_back(5);
			}
			else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != 5)) {
				actions.push_back(4);
				actions.push_back(5);
			}
			else {
				// actions.push_back(3);
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
				}
				if(any_floor_button_pressed_above)
					actions.push_back(1);
				else if(floor1 != 5)
					actions.push_back(5);
				if(any_floor_button_pressed_below)
					actions.push_back(2);
				else if(floor1 != 1)
					actions.push_back(4);
			}
		}
	}
}

void generate_actions(vector<int> &actions, node midway_node) {
	vector<int> action1;
	vector<int> action2;
	get_action1(action1, midway_node);
	get_action2(action2, midway_node);
	cerr<<"THE SIZE OF ACTION 1 = "<<action1.size();
	cerr<<"THE SIZE OF ACTION 2 = "<<action2.size();

	for(int i = 0; i < action1.size(); i++) {
		for(int j = 0; j < action2.size(); j++) {
			int encoded = action1[i]*10 + action2[j];
			actions.push_back(encoded);
		}
	}
} 

// PERFORM ACTION
node perform_output_action(int action, node midway_node) {
	node final_node(midway_node);
	int action1 = action/10;
	int action2 = action%10;
	float expected_no_of_people = 0;
	for(int i = 1; i <= N; i++) {
		if(i != 5)
			expected_no_of_people += midway_node.mystate.getProbFloUp(i);
		if(i != 1)
			expected_no_of_people += midway_node.mystate.getProbFloDown(i);
		expected_no_of_people += midway_node.mystate.getProbElev(i, 1);
		expected_no_of_people += midway_node.mystate.getProbElev(i, 2);
	}
	total_cost += WAIT_TIME_COST_FACTOR*expected_no_of_people;
	switch(action1) {
		case 1: {
 			total_cost += UP_DOWN_COST_FACTOR;
 			final_node.mystate.setFloor(1, midway_node.mystate.getFloor(1) + 1);
 			final_node.mystate.closeDoor(1);
 			break;
		}
		case 2: {
 			total_cost += UP_DOWN_COST_FACTOR;
 			final_node.mystate.setFloor(1, midway_node.mystate.getFloor(1) - 1);
 			final_node.mystate.closeDoor(1);
 			break;
		}
		case 3: {
				final_node.mystate.closeDoor(1);
				break;
			}
		case 4: {
			final_node.mystate.turnOffElevatorButton(midway_node.mystate.getFloor(1), 1);
			final_node.mystate.openDoor(1);
			final_node.mystate.setDirection(1, 1);
			break;
		}
		case 5: {
			final_node.mystate.turnOffElevatorButton(midway_node.mystate.getFloor(1), 1);
			final_node.mystate.openDoor(1);
			final_node.mystate.setDirection(1, 0);
			break;
		}
	}

	switch(action2) {
		case 1: {
 			total_cost += UP_DOWN_COST_FACTOR;
 			final_node.mystate.setFloor(2, midway_node.mystate.getFloor(2) + 1);
 			final_node.mystate.closeDoor(2);
 			break;
		}
		case 2: {
 			total_cost += UP_DOWN_COST_FACTOR;
 			final_node.mystate.setFloor(2, midway_node.mystate.getFloor(2) - 1);
 			final_node.mystate.closeDoor(2);
 			break;
		}
		case 3: {
			final_node.mystate.closeDoor(2);
			break;
		}
		case 4: {
			final_node.mystate.turnOffElevatorButton(midway_node.mystate.getFloor(2), 2);
			final_node.mystate.openDoor(2);
			final_node.mystate.setDirection(2, 1);
			break;
		}
		case 5: {
			final_node.mystate.turnOffElevatorButton(midway_node.mystate.getFloor(2), 2);
			final_node.mystate.openDoor(2);
			final_node.mystate.setDirection(2, 0);
			break;
		}
	}
	return final_node;
}


void go1(int offset, int k) {
  if (k == 0) {
    all_combinations1.push_back(combination1);
    comb_count1++;
    return;
  }
  for (int i = offset; i <= buttons1.size() - k; ++i) {
    combination1.push_back(buttons1[i]);
    go1(i+1, k-1);
    combination1.pop_back();
  }
}

void go2(int offset, int k) {
  if (k == 0) {
    all_combinations2.push_back(combination2);
    comb_count2++;
    return;
  }
  for (int i = offset; i <= buttons2.size() - k; ++i) {
    combination2.push_back(buttons2[i]);
    go2(i+1, k-1);
    combination2.pop_back();
  }
}

vector<string> generate_input_actions(node curr_node) {
	vector<vector<string>> actions;
	srand(time(0));
	cerr<<"IN generate_input_actions"<<endl;
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
			actions.push_back(input_action);
			cerr<<"PUSHING IN 0"<<endl;
		}
		else if (i != 0) {
			int the_floor = i/2 + 1;
			int the_direction = i%2;
			if(curr_node.mystate.floorButtonPressed(the_floor, the_direction))
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
				actions.push_back(input_action);
			}
			cerr<<"PUSHING IN "<<i<<endl;
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
	}
	cerr<<"Choosing random bid"<<endl;
	cerr<<"actions size = "<<actions.size();
	int random = rand()%actions.size();
	return actions[random];
}

int best_policy(node midway_node) {
	return 33;
}

vector<string> translate_actions(int n) {
	vector<string> actions;
	int action1 = n/10;
	int action2 = n%10;
	switch(action1) {
		case 1 : {
			actions.push_back("AU1");
			break;
		}
		case 2 : {
			actions.push_back("AD1");
			break;
		}
		case 3 : {
			actions.push_back("AS1");
			break;
		}
		case 4 : {
			actions.push_back("AOU1");
			break;
		}
		case 5 : {
			actions.push_back("AOD1");
			break;
		}
	}
	switch(action2) {
		case 1 : {
			actions.push_back("AU2");
			break;
		}
		case 2 : {
			actions.push_back("AD2");
			break;
		}
		case 3 : {
			actions.push_back("AS2");
			break;
		}
		case 4 : {
			actions.push_back("AOU2");
			break;
		}
		case 5 : {
			actions.push_back("AOD2");
			break;
		}
	}
	return actions;
}

vector<string> get_best_action(vector<string> input_actions, node &current_node) {
	cerr<<"in best action"<<endl;
	node midway_node(perform_input_action(input_actions, current_node));
	cerr<<"performed input action"<<endl;
	vector<int> actions;
	generate_actions(actions, midway_node);
	int action_size = actions.size();
	cerr<<"Actions generated"<<endl;
	if(action_size == 0)
		cerr<<"INVALID"<<endl;
	else 
		cerr<<"Size of actions is" << action_size;
	for(int i = 0; i < action_size; i++) {
		cerr<<"Action is = "<<actions[i]<<endl;
	}
	cerr<<"SO far so good"<<endl;
	if(action_size == 1) {
		current_node = perform_output_action(actions[0], midway_node);
		return translate_actions(actions[0]);
	}
	else {
		cerr<<"Defining weights"<<endl;
		float current_cost = total_cost;
		float *weights;
		weights = new float[action_size];
		for(int i = 0; i < action_size; i++) {
			total_cost = current_cost;
			node temp_node(perform_output_action(actions[i], midway_node));
			cerr<<"temp_node created "<<i<<endl;
			for(int j = 0; j < rollout; j++) {
				cerr<<"Getting random obserrvation"<<endl;
				vector<string> inputed_action = generate_input_actions(temp_node);
				cerr<<"Performing input action"<<endl;
				temp_node = perform_input_action(inputed_action, temp_node);
				cerr<<"Obtain best policy"<<endl;
				int best_action = best_policy(temp_node);
				cerr<<"Performing best policy"<<endl;
				temp_node = perform_output_action(best_action, temp_node);
			}
			weights[i] = total_cost;
		}
		int index, min = INT_MAX;
		for(int i = 0; i < actions.size(); i++) {
			if(weights[i] < min) {
				min = weights[i];
				index = i;
			}
		}
		total_cost = current_cost;
		current_node = perform_output_action(actions[index], midway_node);
		return translate_actions(actions[index]);
	}	
}

void split(string input, vector<string> &input_actions) {
	// Split input string
		cerr<<"Inside split"<<endl;
		string delimiter = " ";
		size_t pos = 0;
		string token;
		while ((pos = input.find(delimiter)) != string::npos) {
			cerr<<"Running while"<<endl;
		    token = input.substr(0, pos);
		    input_actions.push_back(token);
		    input.erase(0, pos + delimiter.length());
		}
}

int main(int argc, char* argv[]) {
	cerr<<"Lets get started Ankush!";
	N = stoi(argv[1]);
	K = stoi(argv[2]);
	p = stof(argv[3]);
	q = stof(argv[4]);
	r = stof(argv[5]);
	t = stoi(argv[6]);
	string input;
	node current_node;
	cout<<"0\n";
	do {
		cin>>input;
		input = input + " end";
		cerr<<"INPUT = "<<input<<endl;
		vector<string> input_actions;
		vector<string> output_actions;
		split(input, input_actions);
		for(int i = 0; i < input_actions.size(); i++) {
			cerr<<"ACTION = "<<input_actions[i]<<endl;
		}
		cerr<<input_actions.size()<<endl;
		output_actions = get_best_action(input_actions, current_node);
		cerr<<output_actions[0]<<" "<<output_actions[1]<<endl;
		cout<<output_actions[0]<<" "<<output_actions[1]<<endl;
	} while(input != "DONE");
	
	return 0;
}