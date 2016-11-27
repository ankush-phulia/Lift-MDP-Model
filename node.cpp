#include "node.h"

#define N_state 100
#define K_state 6

state::state(){
	
	//initialise elevators
	elevators = new elevator[K_state];
	for (int i = 0;i<K_state;i++){
		elevators[i].position = 1;
		elevators[i].dir = 2;
		elevators[i].open = false;
		elevators[i].elevatorButtons = new bool[N_state];
		elevators[i].elevatorDistribution = new int[N_state];
		for (int j = 0;j<N_state;j++){
			elevators[i].elevatorButtons[j] = false;
			elevators[i].elevatorDistribution[j] = 0;			
		}

	}

	//initialise floor buttons/distributions
	floorButtons = new bool*[N_state];
	floorDistribution = new int*[N_state];
	for (int i = 0;i<N_state;i++){
		floorButtons[i] = new bool[2];
		floorButtons[i][0] = false;
		floorButtons[i][1] = false;
		floorDistribution[i] = new int[2];
		floorDistribution[i][0] = 0;
		floorDistribution[i][1] = 0;
	}

    this->normalise = 100;
}
