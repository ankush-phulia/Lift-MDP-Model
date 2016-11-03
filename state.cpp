#include "state.h"

state::state(){

	this->elevator = 0;
	setFloor(1,1);
	setFloor(2,1);
    for (int i = 0; i<5; i++){
    	for(int j = 0; j<2; j++){
    		floor_dist[i][j] = 0;
    		e_dist[i][j] = 0;
    	}
    }

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
	// std::cerr<<"***SETTING PROB ELEV = "<<newProb<<std::endl;
	e_dist[n-1][k-1] = newProb;
}

void state::setProbFloUp(int n, int newProb){
	// std::cerr<<"***SETTING PROB FLOOR UP = "<<newProb<<std::endl;
	floor_dist[n-1][1] = newProb;
}

void state::setProbFloDown(int n, int newProb){
	// std::cerr<<"***SETTING PROB FLOOR DOWN = "<<newProb<<std::endl;
	floor_dist[n-1][0] = newProb;
}