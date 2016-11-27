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

static int N, K, er; 
static float p, q, r, t;
const int WAIT_TIME_COST_FACTOR = 2;
const int UP_DOWN_COST_FACTOR = 1;
static float total_cost = 0; 
static int rollout = 6;
static int loop_parameter = 25;

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
		// cerr<<"Copy constructor starting"<<endl;
		mystate.elevator = new_state.mystate.elevator;
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < K; j++){
				mystate.e_dist[i][j] = new_state.mystate.e_dist[i][j];
				mystate.floor_dist[i][j] = new_state.mystate.floor_dist[i][j];
			}
		}
		mystate.normalise = new_state.mystate.normalise;
		// cerr<<"Copy constructor finished"<<endl;
	}

	node() {

	}
};

void print_prob(state mystate) {
	cerr<<endl<<endl;
	cerr<<"PRINTING THE PROB"<<endl;
	cerr<<"FOR elevator 1"<<endl;
	for(int i = 1; i <= N; i++) {
		cerr<<"Floor "<<i<<" = "<<mystate.getProbElev(i, 1)<<endl;
	}
	cerr<<"FOR elevator 2"<<endl;
	for(int i = 1; i <= N; i++) {
		cerr<<"Floor "<<i<<" = "<<mystate.getProbElev(i, 2)<<endl;
	}
	cerr<<"FOR floors"<<endl;
	for(int i = 1; i <= N; i++) {
		if(i != 1)
			cerr<<"Floor "<<i<<" down = "<<mystate.getProbFloDown(i)<<endl;
		if(i != N)
			cerr<<"Floor "<<i<<" up = "<<mystate.getProbFloUp(i)<<endl;
	}
	cerr<<endl;
}

// APPLY BUTTON PUSHES
node perform_input_action(vector<string> actions, node current_node) {
	// cerr<<"Inside perform input action"<<endl;
	// print_prob(current_node.mystate);
	node midway_node(current_node);
	vector<int> floor_buttons;
	vector<string> elevator1_button_actions;
	vector<string> elevator2_button_actions;
	int buttons_pressed1 = 0;
	int buttons_pressed2 = 0;
	int floor1_elev1_flag = 0;
	int floor1_elev2_flag = 0;
	// cerr<<"Elevator 1 buttons pressed : ";
	for(int i = 1; i <= N; i++) {
		if(midway_node.mystate.elevatorButtonPressed(1,i)) {
			// cerr<<i<<" ";
		}
	}
	// cerr<<endl;
	// cerr<<"Elevator 2 buttons pressed : ";
	for(int i = 1; i <= N; i++) {
		if(midway_node.mystate.elevatorButtonPressed(2,i)) {
			// cerr<<i<<" ";
		}
	}
	// cerr<<endl;
	for(int i = 1; i < actions.size(); i++) {
		if(actions[i][4] == '1') {
			if(actions[i][2] == '1')
				floor1_elev1_flag = 1;
			int floor_no = actions[i][2] - '0';
			elevator1_button_actions.push_back(actions[i]);
			// cerr<<"New button push detected "<<actions[i]<<endl;
			buttons_pressed1++;
			midway_node.mystate.turnOnElevatorButton(floor_no, 1);
		}
		else if(actions[i][4] == '2') {
			if(actions[i][2] == '1')
				floor1_elev2_flag = 1;
			int floor_no = actions[i][2] - '0';
			elevator2_button_actions.push_back(actions[i]);
			// cerr<<"New button push detected "<<actions[i]<<endl;
			buttons_pressed2++;
			midway_node.mystate.turnOnElevatorButton(floor_no, 2);
		}
	}
	// cerr<<"Elevator buttons turned on"<<endl;
	// cerr<<"buttons_pressed1 = "<<buttons_pressed1<<endl;
	// cerr<<"buttons_pressed2 = "<<buttons_pressed2<<endl;
	// cerr<<"checking if door 1 is open"<<endl;
	if(midway_node.mystate.isOpen(1)) {
		// cerr<<"Inside open 1"<<endl;
		int floor = midway_node.mystate.getFloor(1);
		int direction = midway_node.mystate.getDirection(1)%3;
		midway_node.mystate.turnOffFloorButton(floor, direction);
		float floor_prob;
		vector<int> elevator_buttons;
		if(direction) {
			floor_prob = midway_node.mystate.getProbFloUp(floor);
			midway_node.mystate.setProbFloUp(floor, 0);
			for(int i = floor+1; i <= N; i++) {
				if(midway_node.mystate.elevatorButtonPressed(1, i))
					elevator_buttons.push_back(i);
			}
		}
		else {
			floor_prob = midway_node.mystate.getProbFloDown(floor);
			midway_node.mystate.setProbFloDown(floor, 0);
			for(int i = 1; i < floor; i++) {
				if(midway_node.mystate.elevatorButtonPressed(1, i)){
					elevator_buttons.push_back(i);
					if(i == 1) 
					floor1_elev1_flag = 2;
				}
			}
		}
		int size = elevator_buttons.size();
		if(buttons_pressed1 == 0) {
			for(int i = 0; i < size; i++) {
				float elev_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 1);
				float prob = elev_prob + floor_prob / size;
				midway_node.mystate.setProbElev(1, elevator_buttons[i], round(prob*100));
			}
		}
		else {
			if(floor_prob > buttons_pressed1) {
				int total_buttons_pressed = buttons_pressed1 + elevator_buttons.size();
				floor_prob = floor_prob - buttons_pressed1;
				for(int i = 0; i < buttons_pressed1; i++) {
					int floor_no = elevator1_button_actions[i][2] - '0';
					float elev_floor_prob = midway_node.mystate.getProbElev(floor_no, 1);
					if(floor_no == 1)
						elev_floor_prob += (1.0 + floor_prob*r);
					else if(floor1_elev1_flag && total_buttons_pressed != 1)
						elev_floor_prob += (1.0 + floor_prob*(1-r)/(total_buttons_pressed-1));
					else
						elev_floor_prob += (1.0 + floor_prob/(total_buttons_pressed));
					midway_node.mystate.setProbElev(1, floor_no, round(elev_floor_prob*100));
					// cerr<<"TURN ON ELEVATOR BUTTON "<<floor_no<<endl;
				}
				for(int i = 0; i < elevator_buttons.size(); i++) {
					float elev_floor_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 1);
					if(elevator_buttons[i] == 1)
						elev_floor_prob += (floor_prob*r);
					else if(floor1_elev1_flag && total_buttons_pressed != 1)
						elev_floor_prob += (floor_prob*(1-r)/(total_buttons_pressed-1));
					else
						elev_floor_prob += (floor_prob/(total_buttons_pressed));
					midway_node.mystate.setProbElev(1, elevator_buttons[i], round(elev_floor_prob*100));
				}
			}
			else {
				for(int i = 0; i < buttons_pressed1; i++) {
					int floor_no = elevator1_button_actions[i][2] - '0';
					float elev_floor_prob = midway_node.mystate.getProbElev(floor_no, 1);
					elev_floor_prob += (1.0);
					midway_node.mystate.setProbElev(1, floor_no, round(elev_floor_prob*100));
					// cerr<<"TURN ON ELEVATOR BUTTON "<<floor_no<<endl;
				}
			}
		}
		midway_node.mystate.setProbElev(1, floor, 0);
	}
	// cerr<<"checking if door 2 is open"<<endl;
	if(midway_node.mystate.isOpen(2)) {
		// cerr<<"Inside open 2"<<endl;
		int floor = midway_node.mystate.getFloor(2);
		int direction = midway_node.mystate.getDirection(2)%3;
		midway_node.mystate.turnOffFloorButton(floor, direction);
		float floor_prob;
		vector<int> elevator_buttons;
		if(direction) {
			floor_prob = midway_node.mystate.getProbFloUp(floor);
			midway_node.mystate.setProbFloUp(floor, 0);
			for(int i = floor+1; i <= N; i++) {
				if(midway_node.mystate.elevatorButtonPressed(2, i))
					elevator_buttons.push_back(i);
			}
		}
		else {
			floor_prob = midway_node.mystate.getProbFloDown(floor);
			midway_node.mystate.setProbFloDown(floor, 0);
			for(int i = 1; i < floor; i++) {
				if(midway_node.mystate.elevatorButtonPressed(2, i)) {
					elevator_buttons.push_back(i);
					if(i == 1) 
						floor1_elev2_flag = 2;
				}
			}
		}
		// cerr<<"Get to size"<<endl;
		int size = elevator_buttons.size();
		if(buttons_pressed2 == 0) {
			for(int i = 0; i < size; i++) {
				float elev_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 2);
				float prob = elev_prob + floor_prob / size;
				midway_node.mystate.setProbElev(2, elevator_buttons[i], round(prob*100));
			}
		}
		else {
			// cerr<<"Some buttons are pressed"<<endl;
			if(floor_prob > buttons_pressed2) {
				int total_buttons_pressed = buttons_pressed2 + elevator_buttons.size();
				floor_prob = floor_prob - buttons_pressed2;
				// cerr<<"Get passed the first for loop"<<endl;
				for(int i = 0; i < buttons_pressed2; i++) {
					int floor_no = elevator2_button_actions[i][2] - '0';
					float elev_floor_prob = midway_node.mystate.getProbElev(floor_no, 2);
					if(floor_no == 1)
						elev_floor_prob += (1.0 + floor_prob*r);
					else if(floor1_elev2_flag && total_buttons_pressed != 1)
						elev_floor_prob += (1.0 + floor_prob*(1-r)/(total_buttons_pressed-1));
					else
						elev_floor_prob += (1.0 + floor_prob/(total_buttons_pressed));
					midway_node.mystate.setProbElev(2, floor_no, round(elev_floor_prob*100));
				}
				// cerr<<"Get passed the first for loop"<<endl;
				for(int i = 0; i < elevator_buttons.size(); i++) {
					float elev_floor_prob = midway_node.mystate.getProbElev(elevator_buttons[i], 2);
					if(elevator_buttons[i] == 1)
						elev_floor_prob += (floor_prob*r);
					else if(floor1_elev2_flag && total_buttons_pressed != 1)
						elev_floor_prob += (floor_prob*(1-r)/(total_buttons_pressed-1));
					else
						elev_floor_prob += (floor_prob/(total_buttons_pressed));
					midway_node.mystate.setProbElev(2, elevator_buttons[i], round(elev_floor_prob*100));
				}
			}
			else {
				// cerr<<"Get passed the else"<<endl;
				for(int i = 0; i < buttons_pressed2; i++) {
					int floor_no = elevator2_button_actions[i][2] - '0';
					float elev_floor_prob = midway_node.mystate.getProbElev(floor_no, 2);
					elev_floor_prob += (1.0);
					midway_node.mystate.setProbElev(2, floor_no, round(elev_floor_prob*100));
				}
			}
		}
		midway_node.mystate.setProbElev(2, floor, 0);
	}
	// cerr<<"first actions"<<endl;
	if(actions[0][0] == '0') {
		// cerr<<"zero action"<<endl;
		bool flag = false;
		for(int j = 1; j <= N; j++) {
			if(midway_node.mystate.floorButtonPressed(j, 0) && j != 1)
				floor_buttons.push_back(2*(j-1));
			if(midway_node.mystate.floorButtonPressed(j, 1) && j != N) {
				floor_buttons.push_back(2*j - 1);
				if(j == 1)
					flag = true;
			}
		}
		// cerr<<"Out of loop"<<endl;
		int size = floor_buttons.size();
		for(int j = 0; j < size; j++) {
			int floor = floor_buttons[j]/2 + 1;
			if(floor_buttons[j]%2 == 1) {
				float prob = midway_node.mystate.getProbFloUp(floor), new_prob;
				if(flag) {
					if(floor == 1) {
						new_prob = prob + p*q;
						// cerr<<"New prob = " << new_prob<<endl;
					}
					else {
						new_prob = prob + p*(1-q)/(size-1);
						// cerr<<"New prob = " << new_prob<<endl;
					}
				}
				else {
					new_prob = prob + p/size;
					// cerr<<"New prob = " << new_prob<<endl;
				}
				midway_node.mystate.setProbFloUp(floor, round(new_prob*100));
			}
			else {
				float prob = midway_node.mystate.getProbFloDown(floor), new_prob;
				if(flag)
					new_prob = prob + p*(1-q)/(size-1);
				else 
					new_prob = prob + p/size;
				midway_node.mystate.setProbFloDown(floor, round(new_prob*100));
			}
		}
		// cerr<<"Out of other loop"<<endl;
	}
	else if(actions[0][0] == 'B') {
		// cerr<<"B wala action"<<endl;
		if(actions[0][1] == 'U') {
			int floor = actions[0][3] - '0';
			midway_node.mystate.turnOnFloorButton(floor, 1);
			midway_node.mystate.setProbFloUp(floor, 100);
		}
		else if(actions[0][1] == 'D') {
			int floor = actions[0][3] - '0';
			midway_node.mystate.turnOnFloorButton(floor, 0);
			midway_node.mystate.setProbFloDown(floor, 100);
		}	
	}
	// cerr<<"Returning node"<<endl;
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
	// cerr<<"The floor of elevator 1 = "<<floor1<<endl;
	// cerr<<"The direction of elevator 1 = "<<direction1<<endl;
	if(midway_node.mystate.elevatorButtonPressed(1, floor1)) {
		// cerr<<"We are on the output floor"<<endl;
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
				actions.push_back(1);
			}
			else {
				if((midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != N) && (!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)){
						actions.push_back(4);
				}
				else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == N)){
						actions.push_back(5);
				}	
				else{
					if(floor1 != N)
						actions.push_back(4);
					if(floor1 != 1)
						actions.push_back(5);
				}
			}
		}
		if(!direction1) {
			bool pressed = false;
			for(int i = 1; i < floor1; i++) {
				if(midway_node.mystate.elevatorButtonPressed(1, i)) {
					pressed = true;
					break;
				}
			}
			if(pressed) {
				actions.push_back(5);
				actions.push_back(2);
			}
			else {
				if((midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != N) && (!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)){
						actions.push_back(4);
				}
				else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == N)) {
						actions.push_back(5);
				}	
				else{
					if(floor1 != N)
						actions.push_back(4);
					if(floor1 != 1)
						actions.push_back(5);
				}
			}
		}
	}
	else {
		// cerr<<"NOT on the output floor"<<endl;
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
		if(pressed_up && floor1 != N) {
			// cerr<<"Pressed up is true"<<endl;
			if(midway_node.mystate.floorButtonPressed(floor1, 1)) {
				actions.push_back(4);
				if(q > 0.8 || q < 0.2)
					actions.push_back(1);
			}
			else {
				actions.push_back(1);
			}
		}
		if(pressed_down && floor1 != 1) {
			// cerr<<"Pressed down is true"<<endl;
			if(midway_node.mystate.floorButtonPressed(floor1, 0)) {
				actions.push_back(5);
				if(q > 0.8 || q < 0.2)
					actions.push_back(2);
			}
			else {
				actions.push_back(2);
			}
		}
		else {
			// cerr<<"no button is pressed"<<endl;
			if((midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != N) && (!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)) {
					// cerr<<"heheh111"<<endl;
					actions.push_back(4);
			}
			else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == N)) {
					// cerr<<"heheh222"<<endl;
					actions.push_back(5);
			}
			else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != N)) {
				// cerr<<"heheh333"<<endl;
				actions.push_back(4);
				actions.push_back(5);
			}
			else {
				// actions.push_back(3);
				// cerr<<"heheh444"<<endl;
				bool any_floor_button_pressed_below = false;
				bool any_floor_button_pressed_above = false;
				for(int i = 1; i <= N; i++) {
					if(midway_node.mystate.floorButtonPressed(i, 1) && i != N) {
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
				else if(floor1 != N)
					actions.push_back(4);
				if(any_floor_button_pressed_below && floor1 != 1)
					actions.push_back(2);
				else if(floor1 != 1)
					actions.push_back(5);
			}
		}
	}
}

void get_action2(vector<int> &actions, node midway_node) {
// Lets get some variables from the current state
	int floor1 = midway_node.mystate.getFloor(2);
	int direction1 = midway_node.mystate.getDirection(2)%3;
	if(floor1 > N) {
		floor1 = floor1 - 2;
		midway_node.mystate.setFloor(2, floor1);
	}
	// cerr<<"The floor of elevator 2 = "<<floor1<<endl;
	// cerr<<"The direction of elevator 2 = "<<direction1<<endl;
	if(midway_node.mystate.elevatorButtonPressed(2, floor1)) {
		// cerr<<"We are on the output floor"<<endl;
		if(direction1) {
			bool pressed = false;
			for(int i = floor1 + 1; i <= N; i++) {
				if(midway_node.mystate.elevatorButtonPressed(2, i)) {
					pressed = true;
					break;
				}
			}
			if(pressed && floor1 != N) {
				actions.push_back(4);
				actions.push_back(1);
			}
			else {
				if((midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != N) && (!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)){
						actions.push_back(4);
				}
				else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == N)){
						actions.push_back(5);
				}	
				else{
					if(floor1 != N)
						actions.push_back(4);
					if(floor1 != 1)
						actions.push_back(5);
				}
			}
		}
		if(!direction1) {
			bool pressed = false;
			for(int i = 1; i < floor1; i++) {
				if(midway_node.mystate.elevatorButtonPressed(2, i)) {
					pressed = true;
					break;
				}
			}
			if(pressed && floor1 != 1) {
				// cerr<<"INSIDE PRESSED"<<endl;
				actions.push_back(5);
				actions.push_back(2);
			}
			else {
				if((midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != N) && (!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)){
						actions.push_back(4);
				}
				else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == N)) {
						actions.push_back(5);
				}	
				else{
					if(floor1 != N)
						actions.push_back(4);
					if(floor1 != 1)
						actions.push_back(5);
				}
			}
		}
	}
	else {
		// cerr<<"NOT on the output floor"<<endl;
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
		if(pressed_up && floor1 != N) {
			// cerr<<"Pressed up is true"<<endl;
			if(midway_node.mystate.floorButtonPressed(floor1, 1)) {
				actions.push_back(4);
				if(q > 0.8 || q < 0.2)
					actions.push_back(1);
			}
			else {
				actions.push_back(1);
			}
		}
		if(pressed_down && floor1 != 1) {
			// cerr<<"Pressed down is true"<<endl;
			if(midway_node.mystate.floorButtonPressed(floor1, 0)) {
				actions.push_back(5);
				if(q > 0.8 || q < 0.2)
					actions.push_back(2);
			}
			else {
				// cerr<<"PRESS DOWN ON ME"<<endl;
				actions.push_back(2);
			}
		}
		else {
			// cerr<<"no button is pressed"<<endl;
			if((midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != N) && (!midway_node.mystate.floorButtonPressed(floor1, 0) || floor1 == 1)) {
					// cerr<<"heheh111"<<endl;
					actions.push_back(4);
			}
			else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (!midway_node.mystate.floorButtonPressed(floor1, 1) || floor1 == N)) {
					// cerr<<"heheh222"<<endl;
					actions.push_back(5);
			}
			else if((midway_node.mystate.floorButtonPressed(floor1, 0) && floor1 != 1) && (midway_node.mystate.floorButtonPressed(floor1, 1) && floor1 != N)) {
				// cerr<<"heheh333"<<endl;
				actions.push_back(4);
				actions.push_back(5);
			}
			else {
				// actions.push_back(3);
				// cerr<<"heheh444"<<endl;
				bool any_floor_button_pressed_below = false;
				bool any_floor_button_pressed_above = false;
				for(int i = 1; i <= N; i++) {
					if(midway_node.mystate.floorButtonPressed(i, 1) && i != N) {
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
				if(any_floor_button_pressed_above && floor1 != N)
					actions.push_back(1);
				else if(floor1 != N)
					actions.push_back(4);
				if(any_floor_button_pressed_below && floor1 != 1){
					// cerr<<"PUSHING 2"<<endl;
					actions.push_back(2);
				}
				else if(floor1 != 1)
					actions.push_back(5);
			}
		}
	}
}

void generate_actions(vector<int> &actions, node midway_node) {
	vector<int> action1;
	vector<int> action2;
	get_action1(action1, midway_node);
	get_action2(action2, midway_node);
	// cerr<<"THE SIZE OF ACTION 1 = "<<action1.size();
	// cerr<<"THE SIZE OF ACTION 2 = "<<action2.size();

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
		if(i != N)
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
 			final_node.mystate.setDirection(1, 1);
 			break;
		}
		case 2: {
 			total_cost += UP_DOWN_COST_FACTOR;
 			final_node.mystate.setFloor(1, midway_node.mystate.getFloor(1) - 1);
 			final_node.mystate.closeDoor(1);
 			final_node.mystate.setDirection(1, 0);
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
 			final_node.mystate.setDirection(2, 1);
 			break;
		}
		case 2: {
 			total_cost += UP_DOWN_COST_FACTOR;
 			final_node.mystate.setFloor(2, midway_node.mystate.getFloor(2) - 1);
 			final_node.mystate.closeDoor(2);
 			final_node.mystate.setDirection(2, 0);
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
	// cerr<<"inside go1 "<<offset<<" "<<k<<" "<<buttons1.size()<<endl;
  if (k == 0) {
    all_combinations1.push_back(combination1);
    comb_count1++;
    return;
  }
  for (int i = offset; i <= buttons1.size() - k; i++) {
    combination1.push_back(buttons1[i]);
    go1(i+1, k-1);
    combination1.pop_back();
  }
  return;
}

void go2(int offset, int k) {
  if (k == 0) {
    all_combinations2.push_back(combination2);
    comb_count2++;
    return;
  }
  for (int i = offset; i <= buttons2.size() - k; i++) {
    combination2.push_back(buttons2[i]);
    go2(i+1, k-1);
    combination2.pop_back();
  }
  return;
}

vector<string> generate_input_actions(node curr_node) {
	vector<vector<string>> actions;
	// cerr<<"IN generate_input_actions"<<endl;
	all_combinations1.clear();
	all_combinations2.clear();
	int flag1 = 0, flag2 = 0, count = 0;
	if(curr_node.mystate.isOpen(1)) {
		flag1 = 1;
	} 
	if(curr_node.mystate.isOpen(2)) {
		flag2 = 1;
	} 
	for(int i = 0; i <= 2*N - 2; i++) {
		vector<string> input_action;
		if(i == 0) {
			string myaction = "0";
			input_action.push_back(myaction);
			actions.push_back(input_action);
			// cerr<<"PUSHING IN 0"<<endl;
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
					myaction += (char)(i/2 + 1) + '0';
				}
				else {
					myaction = "BU_";
					myaction += (char)(i/2 + 1) + '0';
				}
				input_action.push_back(myaction);
				actions.push_back(input_action);
			}
			// cerr<<"PUSHING IN "<<i<<endl;
		}
		int direction1 = curr_node.mystate.getDirection(1)%3;
		int direction2 = curr_node.mystate.getDirection(2)%3;
		float prob1, prob2;
		if(direction1 == 1)
			prob1 = curr_node.mystate.getProbFloUp(curr_node.mystate.getFloor(1));
		else if(direction1 == 0)
			prob1 = curr_node.mystate.getProbFloDown(curr_node.mystate.getFloor(1));
		if(direction2 == 1)
			prob2 = curr_node.mystate.getProbFloUp(curr_node.mystate.getFloor(2));
		else if(direction2 == 0)
			prob2 = curr_node.mystate.getProbFloDown(curr_node.mystate.getFloor(2));
		if(flag1 == 1) {
			// cerr<<"FLAG1 == true"<<endl;
			buttons1.clear();
			for(int j = 1; j <= N; j++) {
				if(!curr_node.mystate.elevatorButtonPressed(1,j) && ((curr_node.mystate.getDirection(1)%3 == 1 && j > curr_node.mystate.getFloor(1)) || (curr_node.mystate.getDirection(1)%3 == 0 && j < curr_node.mystate.getFloor(1)))) {
					buttons1.push_back(j);
				}
			}
			// cerr<<"Finished for loop = "<<prob1<<endl;
			comb_count1 = 0;
			for(int j = 1; j <= ceil(prob1); j++) {
				combination1.clear();
				if(buttons1.size() != 0)
					go1(0,1);
			}
			// cerr<<"Combinations made"<<endl;
			for(int j = 0; j < comb_count1; j++) {
				vector<string> next_action(input_action);
				for(int k = 0; k < all_combinations1[j].size(); k++) {
					string new_action = "B_";
					new_action += (char)(all_combinations1[j][k]) + '0';
					new_action += "_1";
					next_action.push_back(new_action);
				}
				if(flag2 == 0)
					actions.push_back(next_action);
				else {
					buttons2.clear();
					for(int j = 1; j <= N; j++) {
						if(!curr_node.mystate.elevatorButtonPressed(2,j) && ((curr_node.mystate.getDirection(2)%3 == 1 && j > curr_node.mystate.getFloor(2)) || (curr_node.mystate.getDirection(2)%3 == 0 && j < curr_node.mystate.getFloor(2)))) {
							buttons2.push_back(j);
						}
					}
					comb_count2 = 0;
					for(int j = 1; j <= ceil(prob2); j++) {
						combination2.clear();
						if(buttons2.size() != 0)
							go2(0,1);
					}
					for(int j = 0; j < comb_count2; j++) {
						vector<string> newer_action(next_action);
						for(int k = 0; k < all_combinations2[j].size(); k++) {
							string new_action = "B_";
							new_action += (char)(all_combinations2[j][k]) + '0';
							new_action += "_1";
							newer_action.push_back(new_action);
						}
						actions.push_back(newer_action);
					}
				}
			}
		}
		else if(flag2 == 1) {
			// cerr<<"FLAG2 == true"<<endl;
			buttons2.clear();
			for(int j = 1; j <= N; j++) {
				if(!curr_node.mystate.elevatorButtonPressed(2,j) && ((curr_node.mystate.getDirection(2)%3 == 1 && j > curr_node.mystate.getFloor(2)) || (curr_node.mystate.getDirection(2)%3 == 0 && j < curr_node.mystate.getFloor(2)))) {
					buttons2.push_back(j);
				}
			}
			comb_count2 = 0;
			// cerr<<"first loop"<<prob2<<endl;
			for(int j = 1; j <= ceil(prob2); j++) {
				combination2.clear();
				if(buttons2.size() != 0)
					go2(0,1);
			}
			// cerr<<"second loop"<<endl;
			for(int j = 0; j < comb_count2; j++) {
				vector<string> next_action(input_action);
				for(int k = 0; k < all_combinations2[j].size(); k++) {
					string new_action = "B_";
					new_action += (char)(all_combinations2[j][k]) + '0';
					new_action += "_1";
					next_action.push_back(new_action);
				}
				actions.push_back(next_action);
			}
			// cerr<<"loops done"<<endl;
		}
	}
	// cerr<<"Choosing random bid"<<endl;
	// cerr<<"actions size = "<<actions.size()<<endl;
	for(int i = 0; i < actions.size(); i++) {
		for(int j = 0; j < actions[i].size(); j++) {
			// cerr<<actions[i][j]<<" ";
		}
		// cerr<<endl;
	}
	int random = rand()%actions.size();
	return actions[random];
}

int best_policy(node midway_node) {
	vector<int> actions;
	generate_actions(actions, midway_node);
	// float current_cost = total_cost;
	// int min = INT_MAX;
	// int index;
	// for(int i = 0; i < actions.size(); i++) {
	// 	total_cost = current_cost;
	// 	// cerr<<"creating temp_node"<<endl;
	// 	node temp_node(perform_output_action(actions[i], midway_node));
	// 	vector<string> inputed_action;
	// 	inputed_action.push_back("0");
	// 	temp_node = perform_input_action(inputed_action, temp_node);
	// 	if(total_cost < min){
	// 		index = i;
	// 		min = total_cost;
	// 	}
	// }
	// total_cost = current_cost;
	int random = rand()%actions.size();
	return actions[random];
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
	// cerr<<"in best action"<<endl;
	node midway_node(perform_input_action(input_actions, current_node));
	// cerr<<"performed input action"<<endl;
	vector<int> actions;
	generate_actions(actions, midway_node);
	int action_size = actions.size();
	// cerr<<"Actions generated"<<endl;
	// if(action_size == 0)
	// 	// cerr<<"INVALID"<<endl;
	// else 
		// cerr<<"Size of actions is " << action_size<<endl;
	for(int i = 0; i < action_size; i++) {
		// cerr<<"Action is = "<<actions[i]<<endl;
	}
	// cerr<<"SO far so good"<<endl;
	if(action_size == 1) {
		current_node = perform_output_action(actions[0], midway_node);
		return translate_actions(actions[0]);
	}
	else {
		// cerr<<"Defining weights"<<endl;
		float current_cost = total_cost;
		float *weights;
		weights = new float[action_size];
		for(int i = 0; i < action_size; i++) {
			weights[i] = 0.0;
		}
		for(int k = 0; k < loop_parameter; k++) {
			for(int i = 0; i < action_size; i++) {
				total_cost = current_cost;
				// cerr<<"creating temp_node"<<endl;
				node temp_node(perform_output_action(actions[i], midway_node));
				// cerr<<"temp_node created "<<i<<endl;
				for(int j = 0; j < rollout; j++) {
					// cerr<<"Getting random obserrvation"<<endl;
					vector<string> inputed_action = generate_input_actions(temp_node);
					// cerr<<"Performing input action"<<endl;
					temp_node = perform_input_action(inputed_action, temp_node);
					// cerr<<"Obtain best policy"<<endl;
					int best_action = best_policy(temp_node);
					// cerr<<"Performing best policy"<<endl;
					temp_node = perform_output_action(best_action, temp_node);
				}
				weights[i] += total_cost;
			}
		}
		int index, min = INT_MAX;
		for(int i = 0; i < action_size; i++) {
			if(weights[i]/loop_parameter < min) {
				min = weights[i]/loop_parameter;
				index = i;
			}
		}
		// index = rand()%action_size;
		total_cost = current_cost;
		current_node = perform_output_action(actions[index], midway_node);
		return translate_actions(actions[index]);
	}	
}

void split(string input, vector<string> &input_actions) {
	// Split input string
		// cerr<<"Inside split"<<endl;
		string delimiter = " ";
		size_t pos = 0;
		string token;
		while ((pos = input.find(delimiter)) != string::npos) {
			// cerr<<"Running while"<<endl;
		    token = input.substr(0, pos);
		    input_actions.push_back(token);
		    input.erase(0, pos + delimiter.length());
		}
}

int parse(string s){
	if (s[1] == 'D'){
		return 2;
	}
	else if (s[1] == 'U'){
		return 1;
	}
	else if (s[1] == 'O'){
		if (s[2]=='U'){
			return 4;
		}
		else{
			return 5;
		}
	}
	else{
		return 3;
	}
}

int basePolicy2(int n, int j){
	string link[16];
	for (int i =0;i<4*n-4;i=i+2){
		if (i<2*n-2){
			link[i] = "AU";
			link[i+1] = "AOU";
		}
		else{
			link[i] = "AD";
			link[i+1] = "AOD";
		}
	}
	link[2*n-3] = "AOD";
	link[4*n-5] = "AOU";
	
	string actions[2] = {"AOU1","AOU2"};

	for (int l = 0; l<2; l++){
		if (j>((n/2)+1)*l*2) {
			char c = (char)(l+1) + '0';
			actions[l] = link[(j - (n/2+1)*l*2 - 1) % (4*n-4)] + c;
		}
	}
	return 10*parse(actions[0]) + parse(actions[1]);
}

int get_best_action2(vector<string> input_actions, node &current_node) {
	// cerr<<"in best action"<<endl;
	node midway_node(perform_input_action(input_actions, current_node));
	// cerr<<"performed input action"<<endl;
	// int action = basePolicy(midway_node.mystate, 5, rng);
	int action = basePolicy2(N, er);
	current_node = perform_output_action(action, midway_node);
	return action;
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	// cerr<<"Lets get started Ankush!";
	N = stoi(argv[1]);
	K = stoi(argv[2]);
	p = stof(argv[3]);
	q = stof(argv[4]);
	r = stof(argv[5]);
	t = stoi(argv[6]);
	string input;
	int count = 0;
	node current_node;
	cout<<"0\n";
	// cerr<<"Zero being sent"<<endl;
	er = 0;
	do {
		count++;
		er++;
		getline(cin, input);
		input = input + " end";
		// cerr<<"INPUT = "<<input<<"count = "<<count<<endl;
		vector<string> input_actions;
		vector<string> output_actions;
		split(input, input_actions);
		for(int i = 0; i < input_actions.size(); i++) {
			// cerr<<"ACTION = "<<input_actions[i]<<endl;
		}
		if(q < 0.15 && r > 0.85)
			output_actions = translate_actions(get_best_action2(input_actions, current_node));
		else
			output_actions = get_best_action(input_actions, current_node);
		// cerr<<output_actions[0]<<" "<<output_actions[1]<<endl;
		cout<<output_actions[0]<<" "<<output_actions[1]<<endl;
	} while(input != "DONE");
	
	return 0;
}