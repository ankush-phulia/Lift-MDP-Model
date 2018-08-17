#include "node.h"
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
static int rollout = 1;
static int loop_parameter = 1;
static int threshold = 5;
static bool screwed;

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
		//initialise elevators
		// mystate.elevators = new elevator[K];
		for (int i = 0; i < K; i++){
			mystate.elevators[i].position = new_state.mystate.elevators[i].position;
			mystate.elevators[i].dir = new_state.mystate.elevators[i].dir;
			mystate.elevators[i].open = new_state.mystate.elevators[i].open;
			// mystate.elevators[i].elevatorButtons = new bool[N];
			// mystate.elevators[i].elevatorDistribution = new int[N];
			for (int j = 0; j < N; j++){
				mystate.elevators[i].elevatorButtons[j] = new_state.mystate.elevators[i].elevatorButtons[j];
				mystate.elevators[i].elevatorDistribution[j] = new_state.mystate.elevators[i].elevatorDistribution[j];			
			}

		}
		//initialise floor buttons/distributions
		// mystate.floorButtons = new bool*[N];
		// mystate.floorDistribution = new int*[N];
		for (int i = 0; i < N; i++){
			// mystate.floorButtons[i] = new bool[2];
			mystate.floorButtons[i][0] = new_state.mystate.floorButtons[i][0];
			mystate.floorButtons[i][1] = new_state.mystate.floorButtons[i][1];
			// mystate.floorDistribution[i] = new int[2];
			mystate.floorDistribution[i][0] = new_state.mystate.floorDistribution[i][0];
			mystate.floorDistribution[i][1] = new_state.mystate.floorDistribution[i][1];
		}
	    mystate.normalise = 100;
		// cerr<<"Copy constructor finished"<<endl;
	}

	node() {

	}
};

void print_prob(state &mystate) {
	cerr<<endl<<endl;
	cerr<<"PRINTING THE PROB"<<endl;
	for(int j = 1; j <= K; j++) {
		cerr<<"FOR elevator "<<j<<endl;
		for(int i = 1; i <= N; i++) {
			cerr<<"Floor "<<i<<" = "<<mystate.getProbElev(i, j)<<endl;
		}
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
// APPLY BUTTON PUSHES
void perform_input_action(vector<string> actions, node &midway_node) {
	// cerr<<"Inside perform input action"<<endl;
	// print_prob(current_node.mystate);
	vector<int> floor_buttons;
	vector<string> *elevator_button_actions;
    elevator_button_actions = new vector<string>[K];
	int buttons_pressed[K];
	int floor1_elev_flag[K];
	for(int i = 0; i < K; i++) {
		buttons_pressed[i] = 0;
		floor1_elev_flag[i] = 0;
	}
	for(int j = 1; j <= K; j++) {
		// cerr<<"Elevator "<<j<<" buttons pressed : ";
		for(int i = 1; i <= N; i++) {
			if(midway_node.mystate.elevatorButtonPressed(j,i)) {
				// cerr<<i<<" ";
			}
		}
		// cerr<<endl;
	}
	// cerr<<endl;
	for(int i = 1; i < actions.size(); i++) {
		int floor_no = actions[i][2] - '0';
		int elevator_no = actions[i][4] - '0';
		if(elevator_no <= N && elevator_no >= 1) {
			if(floor_no == 1)
				floor1_elev_flag[elevator_no-1] = 1;
			elevator_button_actions[elevator_no-1].push_back(actions[i]);
			// cerr<<"New button push detected "<<actions[i]<<endl;
			buttons_pressed[elevator_no-1]++;
			midway_node.mystate.turnOnElevatorButton(floor_no, elevator_no);
		}
	}

	// cerr<<"Elevator buttons turned on"<<endl;
	// cerr<<"buttons_pressed1 = "<<buttons_pressed1<<endl;
	// cerr<<"buttons_pressed2 = "<<buttons_pressed2<<endl;
	// cerr<<"checking if door 1 is open"<<endl;
	for(int j = 1; j <= K; j++) {
		if(midway_node.mystate.isOpen(j)) {
			// cerr<<"Inside open 2"<<endl;
			int floor = midway_node.mystate.getFloor(j);
			int direction = midway_node.mystate.getDirection(j)%3;
			midway_node.mystate.turnOffFloorButton(floor, direction);
			float floor_prob;
			vector<int> elevator_buttons;
			if(direction) {
				floor_prob = midway_node.mystate.getProbFloUp(floor);
				midway_node.mystate.setProbFloUp(floor, 0);
				for(int i = floor+1; i <= N; i++) {
					if(midway_node.mystate.elevatorButtonPressed(j, i))
						elevator_buttons.push_back(i);
				}
			}
			else {
				floor_prob = midway_node.mystate.getProbFloDown(floor);
				midway_node.mystate.setProbFloDown(floor, 0);
				for(int i = 1; i < floor; i++) {
					if(midway_node.mystate.elevatorButtonPressed(j, i)) {
						elevator_buttons.push_back(i);
						if(i == 1) 
							floor1_elev_flag[j-1] = j;
					}
				}
			}
			// cerr<<"Get to size"<<endl;
			int size = elevator_buttons.size();
			if(buttons_pressed[j-1] == 0) {
				for(int i = 0; i < size; i++) {
					float elev_prob = midway_node.mystate.getProbElev(elevator_buttons[i], j);
					float prob = elev_prob + floor_prob / size;
					midway_node.mystate.setProbElev(j, elevator_buttons[i], round(prob*100));
				}
			}
			else {
				// cerr<<"Some buttons are pressed"<<endl;
				if(floor_prob > buttons_pressed[j-1]) {
					int total_buttons_pressed = buttons_pressed[j-1] + elevator_buttons.size();
					floor_prob = floor_prob - buttons_pressed[j-1];
					// cerr<<"Get passed the first for loop"<<endl;
					for(int i = 0; i < buttons_pressed[j-1]; i++) {
						int floor_no = elevator_button_actions[j-1][i][2] - '0';
						float elev_floor_prob = midway_node.mystate.getProbElev(floor_no, j);
						if(floor_no == 1)
							elev_floor_prob += (1.0 + floor_prob*r);
						else if(floor1_elev_flag[j-1] && total_buttons_pressed != 1)
							elev_floor_prob += (1.0 + floor_prob*(1-r)/(total_buttons_pressed-1));
						else
							elev_floor_prob += (1.0 + floor_prob/(total_buttons_pressed));
						midway_node.mystate.setProbElev(j, floor_no, round(elev_floor_prob*100));
					}
					// cerr<<"Get passed the first for loop"<<endl;
					for(int i = 0; i < elevator_buttons.size(); i++) {
						float elev_floor_prob = midway_node.mystate.getProbElev(elevator_buttons[i], j);
						if(elevator_buttons[i] == 1)
							elev_floor_prob += (floor_prob*r);
						else if(floor1_elev_flag[j-1] && total_buttons_pressed != 1)
							elev_floor_prob += (floor_prob*(1-r)/(total_buttons_pressed-1));
						else
							elev_floor_prob += (floor_prob/(total_buttons_pressed));
						midway_node.mystate.setProbElev(j, elevator_buttons[i], round(elev_floor_prob*100));
					}
				}
				else {
					// cerr<<"Get passed the else"<<endl;
					for(int i = 0; i < buttons_pressed[j-1]; i++) {
						int floor_no = elevator_button_actions[j-1][i][2] - '0';
						float elev_floor_prob = midway_node.mystate.getProbElev(floor_no, j);
						elev_floor_prob += (1.0);
						midway_node.mystate.setProbElev(j, floor_no, round(elev_floor_prob*100));
					}
				}
			}
			midway_node.mystate.setProbElev(j, floor, 0);
		}
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
void get_action(vector<int> &actions, node &midway_node, int elev) {
	// Lets get some variables from the current state
	int floor1 = midway_node.mystate.getFloor(elev);
	int direction1 = midway_node.mystate.getDirection(elev)%3;
	// cerr<<"The floor of elevator 1 = "<<floor1<<endl;
	// cerr<<"The direction of elevator 1 = "<<direction1<<endl;
	bool am_i_dying = false;
	for(int i = floor1 +1; i <= N; i++) {
		if(midway_node.mystate.getProbElev(i, elev) > threshold) {
			am_i_dying = true;
			break;
		}
	}
	if(am_i_dying) {
		actions.push_back(1);
		if(midway_node.mystate.elevatorButtonPressed(elev, floor1))
			actions.push_back(4);
	}
	else if(midway_node.mystate.elevatorButtonPressed(elev, floor1)) {
		// cerr<<"We are on the output floor"<<endl;
		if(direction1) {
			bool pressed = false;
			for(int i = floor1 + 1; i <= N; i++) {
				if(midway_node.mystate.elevatorButtonPressed(elev, i)) {
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
				if(midway_node.mystate.elevatorButtonPressed(elev, i)) {
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
			if(midway_node.mystate.elevatorButtonPressed(elev, i)) {
				pressed_up = true;
				break;
			}
		}
		for(int i = 1; i < floor1; i++) {
			if(midway_node.mystate.elevatorButtonPressed(elev, i)) {
				pressed_down = true;
				break;
			}
		}
		if(pressed_up && floor1 != N) {
			// cerr<<"Pressed up is true"<<endl;
			if(midway_node.mystate.floorButtonPressed(floor1, 1)) {
				actions.push_back(4);
			}
			else {
				actions.push_back(1);
			}
		}
		if(pressed_down && floor1 != 1) {
			// cerr<<"Pressed down is true"<<endl;
			if(midway_node.mystate.floorButtonPressed(floor1, 0)) {
				actions.push_back(5);
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

void generate_actions(vector<int> &actions, node &midway_node) {
	vector<int> action[100];
	for(int j = 0; j < K; j++) {
		get_action(action[j], midway_node, j+1);
		// cerr<<"action "<<j+1<<"size = "<<action[j].size();
	}

    vector<int> elevatorActions;
    for (int i = 0; i < K; i++) {
    	elevatorActions = action[i];
    	if(actions.size() == 0) {
    		actions = elevatorActions;
    	}
    	else {
    		vector<int> temp;
	        for(int j = 0; j < actions.size(); j++) {
	        	for(int k = 0; k < elevatorActions.size(); k++) {
	        		int semiAction = actions[j]; 
	        		int an_action = elevatorActions[k];
	        		int move = 10*semiAction + an_action;
	        		// cerr<<"Pushing to temp"<<endl;
	                temp.push_back(move);
	        	}
	        }
	        actions = temp;
	    }
        // for (auto& semiAction : actions) {
        //     for (auto an_action : elevatorActions) {
        //     	int move = 10*semiAction + an_action;
        //     	cerr<<"Pushing to temp"<<endl;
        //         temp.push_back(move);
        //     }
        // }
    }

    // cerr<<"Size of actions = "<<actions.size();

    for(int i = 0; i < actions.size(); i++) {
    	// cerr<<"Action generated = "<<actions[i]<<endl;
    }
} 

// PERFORM ACTION
void perform_output_action(int action, node &final_node) {

	int the_action[K];
	for(int j = K-1; j >= 0; j--) {
		the_action[j] = action%10;
		action = action/10;
	}

	float expected_no_of_people = 0;
	for(int i = 1; i <= N; i++) {
		if(i != N)
			expected_no_of_people += final_node.mystate.getProbFloUp(i);
		if(i != 1)
			expected_no_of_people += final_node.mystate.getProbFloDown(i);
		for(int j = 1; j <= K; j++) {
			expected_no_of_people += final_node.mystate.getProbElev(i, j);
		}
	}
	total_cost += WAIT_TIME_COST_FACTOR*expected_no_of_people;
	for(int j = 1; j <= K; j++) {
		switch(the_action[j-1]) {
			case 1: {
	 			total_cost += UP_DOWN_COST_FACTOR;
	 			final_node.mystate.setFloor(j, final_node.mystate.getFloor(j) + 1);
	 			final_node.mystate.closeDoor(j);
	 			final_node.mystate.setDirection(j, 1);
	 			break;
			}
			case 2: {
	 			total_cost += UP_DOWN_COST_FACTOR;
	 			final_node.mystate.setFloor(j, final_node.mystate.getFloor(j) - 1);
	 			final_node.mystate.closeDoor(j);
	 			final_node.mystate.setDirection(j, 0);
	 			break;
			}
			case 3: {
					final_node.mystate.closeDoor(j);
					break;
				}
			case 4: {
				final_node.mystate.turnOffElevatorButton(final_node.mystate.getFloor(j), j);
				final_node.mystate.openDoor(j);
				final_node.mystate.setDirection(j, 1);
				break;
			}
			case 5: {
				final_node.mystate.turnOffElevatorButton(final_node.mystate.getFloor(j), j);
				final_node.mystate.openDoor(j);
				final_node.mystate.setDirection(j, 0);
				break;
			}
		}
	}
	// return final_node;
}

vector<string> generate_input_actions(state &mystate){
    //LOOK at the FLOORs
    std::vector<string>* induvidualActions;
    induvidualActions = new std::vector<string>[K+1];
    string b;
    char c;
    induvidualActions[K].push_back("0");
    for (int i = 0; i<N;i++){//up and down buttons
        if (!mystate.floorButtonPressed(i+1,0) && i!=0){
            c = (char)(i+1) + '0';
            b = "BD_";
            b += c;
            induvidualActions[K].push_back(b);
        }
        if (!mystate.floorButtonPressed(i+1,1) && i!=N-1){
            c = (char)(i+1) + '0';
            b = "BU_";
            b +=c;
            induvidualActions[K].push_back(b);
        }
    }
    //LOOK at each elevator
    int floor;
    bool open;
    uint8_t dir;
    for (int i = 0;i<K;i++){
        floor = mystate.getFloor(i+1);
        dir = mystate.getDirection(i+1);
        open = mystate.isOpen(i+1);
        induvidualActions[i].push_back("");
        if (open){// check only if doors open
            if (!dir){//going down, look at buttons below
                for (int j = floor-2;j>=0;j--){
                    if (!mystate.elevatorButtonPressed(i+1,j+1)){
                        char e = (char)(i+1) + '0';
                        char f = (char)(j+1) + '0';
                        b = "B_";
                        b += f; b+='_'; b+= e;
                        induvidualActions[i].push_back(b);
                    }
                }
            }
            else{//going up, look at floors above
                for (int j = floor;j<N;j++){
                    if (!mystate.elevatorButtonPressed(i+1,j+1)){
                        char e = (char)(i+1) + '0';
                        char f = (char)(j+1) + '0';
                        b = "B_";
                        b += f; b+='_'; b+= e;
                        induvidualActions[i].push_back(b);
                    }
                }
            }
        }        
    }
    std::vector<std::vector<string> > finalActions = {{}};
    std::vector<std::vector<string> > elevatorButtons = {{}};
    std::vector<string> inputActions;
    for (int i = 0; i<K;i++){
        inputActions = induvidualActions[i];
        std::vector<vector<string> > temp;
        for (auto& semiAction : elevatorButtons) {
            for (auto action : inputActions) {
                semiAction.push_back(action);
                temp.push_back(semiAction);
            }
        }
        elevatorButtons.swap(temp);
    }

    for (string floorButton:induvidualActions[K]){
        for (auto& elevatorButtonAction: elevatorButtons){
            elevatorButtonAction.push_back(floorButton);
            finalActions.push_back(elevatorButtonAction);
            elevatorButtonAction.pop_back();
        }
    }

    std::vector<string> temp;
    while(temp.size() == 0) {
    	int random = rand()%finalActions.size();
    	temp = finalActions[random];
    }
    std::vector<string> output_action;
    // cerr<<"The output action is for size =  "<<temp.size()<<" ";
    for(int i = temp.size()-1; i >=0; i--) {
    	if(temp[i] != ""){
    		output_action.push_back(temp[i]);
    		// cerr<<temp[i ]<<" ";
    	}
    }
    // cerr<<endl;
    // cerr<<"Returning from function\n";
    return output_action;

}

int best_policy(node &midway_node) {
	vector<int> actions;
	generate_actions(actions, midway_node);
	int random = rand()%actions.size();
	return actions[random];
}

vector<string> translate_actions(int action) {
	// cerr<<"The action choosen = "<<action<<endl;
	vector<string> actions;
	int the_action[K];
	for(int j = K-1; j >= 0; j--) {
		the_action[j] = action%10;
		action = action/10;
	}
	// cerr<<"the_action is all set"<<endl;
	string move = "";
	for(int j = 1; j <= K; j++) {
		switch(the_action[j-1]) {
			case 1 : {
				move = "AU";
				move += (char)j + '0';
				actions.push_back(move);
				break;
			}
			case 2 : {
				move = "AD";
				move += (char)j + '0';
				actions.push_back(move);
				break;
			}
			case 3 : {
				move = "AS";
				move += (char)j + '0';
				actions.push_back(move);
				break;
			}
			case 4 : {
				move = "AOU";
				move += (char)j + '0';
				actions.push_back(move);
				break;
			}
			case 5 : {
				move = "AOD";
				move += (char)j + '0';
				actions.push_back(move);
				break;
			}
		}
	}
	return actions;
}

vector<string> get_best_action(vector<string> input_actions, node &midway_node) {
	// cerr<<"in best action"<<endl;
	perform_input_action(input_actions, midway_node);
	// cerr<<"performed input action"<<endl;
	vector<int> actions;
	generate_actions(actions, midway_node);
	int action_size = actions.size();
	// cerr<<"Actions generated"<<endl;
	// if(action_size == 0)
	// 	cerr<<"INVALID"<<endl;
	// else 
	// 	cerr<<"Size of actions is " << action_size<<endl;
	// for(int i = 0; i < action_size; i++) {
	// 	// cerr<<"Action is = "<<actions[i]<<endl;
	// }
	// cerr<<"SO far so good"<<endl;
	if(action_size == 1) {
		perform_output_action(actions[0], midway_node);
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
				node temp_node = midway_node;
				perform_output_action(actions[i], temp_node);
				// cerr<<"temp_node created "<<i<<endl;
				for(int j = 0; j < rollout; j++) {
					// cerr<<"Getting random obserrvation"<<endl;
					vector<string> inputed_action = generate_input_actions(temp_node.mystate);
					// cerr<<"Performing input action"<<endl;
					perform_input_action(inputed_action, temp_node);
					// cerr<<"Obtain best policy"<<endl;
					int best_action = best_policy(temp_node);
					// cerr<<"Performing best policy"<<endl;
					perform_output_action(best_action, temp_node);
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
		perform_output_action(actions[index], midway_node);
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

int basePolicy2(int j){
	string *link;
	link = new string[4*N - 4];
	for (int i = 0; i < 4*N-4; i = i+2){
		if (i < 2*N-2){
			link[i] = "AU";
			link[i+1] = "AOU";
		}
		else{
			link[i] = "AD";
			link[i+1] = "AOD";
		}
	}
	link[2*N-3] = "AOD";
	link[4*N-5] = "AOU";
	
	string *actions;
	actions = new string[K] ;

	for (int x = 0; x < K; x++){
		char c = (char)(x+1) + '0';
		string s = "AOU" + c;
		actions[x] = s;
	}

	int final = 0;
	for (int l = 0; l < K; l++){
		if (j > ((N/K)+1)*l*4) {
			char c = (char)(l+1) + '0';
			actions[l] = link[(j - (N/K+1)*l*4 - 1) % (4*N-4)] + c;
		}
		final = final*10+parse(actions[l]);
	}
	return final;
}

int get_best_action2(vector<string> input_actions, node &current_node) {
	// cerr<<"in best action"<<endl;
	// node midway_node(perform_input_action(input_actions, current_node));
	// cerr<<"performed input action"<<endl;
	// int action = basePolicy(midway_node.mystate, 5, rng);
	int action = basePolicy2(er);
	// current_node = perform_output_action(action, midway_node);
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
	screwed = true;
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
		// for(int i = 0; i < input_actions.size(); i++) {
			// cerr<<"ACTION = "<<input_actions[i]<<endl;
		// }
		if(screwed)
			output_actions = translate_actions(get_best_action2(input_actions, current_node));
		else
			output_actions = get_best_action(input_actions, current_node);
		for(int i = 0; i < output_actions.size(); i++) {
			// cerr<<"SENDING out "<<output_actions[0]<<" ";
			cout<<output_actions[i]<<" ";//<<output_actions[1]<<endl;
		}
		cout<<endl;
	} while(input != "DONE");
	
	return 0;
}