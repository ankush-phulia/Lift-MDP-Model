#include "simulate.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

simulate::simulate(int N, int K, float p, float q, float r, int t) {
	this->N = N;
	this->K = K;
	this->p = p;
	this->q = q;
	this->r = r;
	this->t = t;
	this->total_cost = 0;
	this->total_people_served = 0;
}

void simulate::perform_action(int action) {
	srand(time(0));
	int action1 = action/10;
	int action2 = action%10;
	total_cost += WAIT_TIME_COST_FACTOR*people_in_sys.size();
	switch(action1) {
		case 0: {
 			total_cost += UP_DOWN_COST_FACTOR;
 			sim_state.setFloor(1, sim_state.getFloor(1) + 1);
		}
		case 1: {
 			total_cost += UP_DOWN_COST_FACTOR;
 			sim_state.setFloor(1, sim_state.getFloor(1) - 1);
		}
		case 3: {
			sim_state.turnOffElevatorButton(1,sim_state.getFloor(1));
			sim_state.turnOffFloorButton(sim_state.getFloor(1), 1);
			for(int i = 0; i < people_in_sys.size(); i++) {
				if(people_in_sys[i].dest == sim_state.getFloor(1) && people_in_sys[i].elevator_number == 1) {
					remove_person(people_in_sys[i].id);
				}
				if(people_in_sys[i].start == sim_state.getFloor(1) && people_in_sys[i].elevator_number == -1 && people_in_sys[i].direction == 1) {
					people_in_sys[i].elevator_number = 1;
					sim_state.turnOnElevatorButton(people_in_sys[i].dest, 1);
				}
			}
		}
		case 4: {
			sim_state.turnOffElevatorButton(1,sim_state.getFloor(1));
			sim_state.turnOffFloorButton(sim_state.getFloor(1), 0);
			for(int i = 0; i < people_in_sys.size(); i++) {
				if(people_in_sys[i].dest == sim_state.getFloor(1) && people_in_sys[i].elevator_number == 1) {
					remove_person(people_in_sys[i].id);
				}
				if(people_in_sys[i].start == sim_state.getFloor(1) && people_in_sys[i].elevator_number == -1 && people_in_sys[i].direction == 0) {
					people_in_sys[i].elevator_number = 1;
					sim_state.turnOnElevatorButton(people_in_sys[i].dest, 1);
				}
			}
		}
	}
	switch(action2) {
		case 0: {
 			total_cost += UP_DOWN_COST_FACTOR;
 			sim_state.setFloor(2, sim_state.getFloor(2) + 1);
		}
		case 1: {
 			total_cost += UP_DOWN_COST_FACTOR;
 			sim_state.setFloor(2, sim_state.getFloor(2) - 1);
		}
		case 3: {
			sim_state.turnOffElevatorButton(2, sim_state.getFloor(2));
			sim_state.turnOffFloorButton(sim_state.getFloor(2), 1);
			for(int i = 0; i < people_in_sys.size(); i++) {
				if(people_in_sys[i].dest == sim_state.getFloor(2) && people_in_sys[i].elevator_number == 2) {
					remove_person(people_in_sys[i].id);
				}
				if(people_in_sys[i].start == sim_state.getFloor(2) && people_in_sys[i].elevator_number == -1 && people_in_sys[i].direction == 1) {
					people_in_sys[i].elevator_number = 2;
					sim_state.turnOnElevatorButton(people_in_sys[i].dest, 2);
				}
			}
		}
		case 4: {
			sim_state.turnOffElevatorButton(2,sim_state.getFloor(2));
			sim_state.turnOffFloorButton(sim_state.getFloor(2), 0);
			for(int i = 0; i < people_in_sys.size(); i++) {
				if(people_in_sys[i].dest == sim_state.getFloor(2) && people_in_sys[i].elevator_number == 2) {
					remove_person(people_in_sys[i].id);
				}
				if(people_in_sys[i].start == sim_state.getFloor(2) && people_in_sys[i].elevator_number == -1 && people_in_sys[i].direction == 0) {
					people_in_sys[i].elevator_number = 2;
					sim_state.turnOnElevatorButton(people_in_sys[i].dest, 1);
				}
			}
		}
	}
	int random = rand()%100 + 1;
	if(random < p*100) {
		total_people_served++;
		add_person(total_people_served, N, q, r);
	}
}

state simulate::return_midstate() {
	return sim_state;
}

void simulate::print_state() {
	// Choose the representation
	cout<<"Position of lift 1 "<<sim_state.getFloor(1)<<endl;
	cout<<"Position of lift 2 "<<sim_state.getFloor(2)<<endl;
	cout<<"No of people in the system "<<people_in_sys.size()<<endl;
	for(int i = 1; i <= N; i++) {
		cout<<"At floor "<< i<< endl;
		if(sim_state.floorButtonPressed(i, 0))
			cout<<"Button is down"<< endl;
		if(sim_state.floorButtonPressed(i, 1))
			cout<<"Button is up"<< endl;	 
	}
	for(int i = 1; i <= 2; i++) {
		cout<<"For lift "<<i<<endl;
		cout<<"No of buttons pressed is : ";
		for(int j = 1; j <= N; j++){
			if(sim_state.elevatorButtonPressed(i, j))
				cout<<" "<<j;
		}
		cout<<endl;
	}	
}

void simulate::remove_person(int id) {
	int index = -1;
	for(int i = 0; i < people_in_sys.size(); i++) {
		if(people_in_sys[i].id == id) {
			index = i;
			break;
		}
	}
	people_in_sys.erase(people_in_sys.begin() + index);
}

void simulate::add_person(int id, int N, float q, float r) {
	person new_person(id, N, q, r);
	people_in_sys.push_back(new_person);
	sim_state.turnOnFloorButton(new_person.start, new_person.direction);
}