#include "state.h"

state::state(){

	this->elevator = 0;
	setFloor(1,1);
	setFloor(2,1);
	this->floor_dist = new uint32_t[5];
    for (int i = 0; i<5; i++){
    	// initialise floor distribution probs
    	floor_dist[i] = 0;
    }

    // initialise elevator distributions after this too
    this->e1_dist = 0;
    this->e2_dist = 0;

    this->normalise = 100;
}

void state::setFloor(int k, int n){
	int mask = (4294967295)^(7<<(3*k-3));
	elevator &= mask;
	elevator |= (n<<(3*k-3));
}


void state::setDirection(int k, int dir){
	//10 for static, 01 for up, 00 for down
	int mask = (4294967295)^(3<<(26+2*k-2));
	elevator &= mask;
	elevator |= (dir<<(26+2*k-2));
}

void state::setProbElev(int k, int n, int newProb){
	int mask = (4294967295)^(4095<<(12*n-12));
	if (k==1){
		e1_dist &= mask;
		e1_dist |= (newProb<<(12*n-12));
	}
	else{
		e2_dist &= mask;
		e2_dist |= (newProb<<(12*n-12));
	}
}

void state::setProbFloUp(int n, int newProb){
	int mask = (4294967295)^(4095);
	floor_dist[n-1] &= mask;
	floor_dist[n-1] |= (newProb);
}

void state::setProbFloDown(int n, int newProb){
	int mask = (4294967295)^(4095<<12);
	floor_dist[n-1] &= mask;
	floor_dist[n-1] |= (newProb<<12);
}