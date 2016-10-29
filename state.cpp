#include "state.h"

state::state(int n){

	this->elevator = 0;

	this->floor_dist = new uint32_t [n];
    for (int i = 0;i<n;i++){
    	//initialise floor distribution probs
    }

    //initialise elevator distributions after this too
    this->e0_dist = 0;
    this->e1_dist = 0;

    this->normalise = n*100;
}

void state::setFloor(int k, int n){
	int mask = (4294967295)^(7<<(3*k));
	elevator &= mask;
	elevator |= (n<<(3*k));
}


void state::setDirection(int k, int dir){
	//00 for static, 01 for up, 10 for down
	int mask = (4294967295)^(3<<(23+2*k));
	elevator &= mask;
	elevator |= (dir<<(23+2*k));
}

void state::setProbElev(int k, int n, int newProb){
	int mask = (4294967295)^(4095<<(12*n));
	if (!k){
		e0_dist &= mask;
		e0_dist |= (newProb<<(12*n));
	}
	else{
		e1_dist &= mask;
		e1_dist |= (newProb<<(12*n));
	}
}

void state::setProbFloUp(int n, int newProb){
	int mask = (4294967295)^(4095);
	floor_dist[n] &= mask;
	floor_dist[n] |= (newProb);
}

void state::setProbFloDown(int n, int newProb){
	int mask = (4294967295)^(4095<<12);
	floor_dist[n] &= mask;
	floor_dist[n] |= (newProb<<12);
}