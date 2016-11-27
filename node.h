#ifndef NODE_H
#define NODE_H
#include <stdlib.h>

using namespace std;

struct elevator{
	int position;
	bool open;
	uint8_t dir;
	bool* elevatorButtons;
	int* elevatorDistribution;
};


class state{

public:
	elevator* elevators;
	bool** floorButtons;
	int** floorDistribution;
	int normalise;

	state();

	state(const state &new_state);

	//methods for retrieving elevators
	inline int getFloor(int k){
		//elevator 1 or 2
		return elevators[k-1].position;
	}
	inline bool isOpen(int k){
		return elevators[k-1].open;
	}
	inline int getDirection(int k){
		return elevators[k-1].dir;
	}
	inline bool elevatorButtonPressed(int k, int n){
		//k = 1,2 ; n = 1,2,3,4,5
		return elevators[k-1].elevatorButtons[n-1];
	}
	inline bool floorButtonPressed(int n, int upordown){
		//make upordown bool??!!
		//careful when using button for floor 1 or 5, always give upordown as 0 or false
		return floorButtons[n-1][upordown];
	}

	//methods for changing elevators
	inline void setFloor(int k, int n){
		elevators[k-1].position = n;
	}

	inline void openDoor(int k){
		elevators[k-1].open = true;
	}

	inline void closeDoor(int k){
		elevators[k-1].open = false;
	}

	//10 for static, 01 for up, 00 for down
	void setDirection(int k, int dir){
		elevators[k-1].dir = dir;
	}

	inline void turnOnFloorButton(int n, int upordown){
		floorButtons[n-1][upordown] = true;
	}

	inline void turnOffFloorButton(int n, int upordown){
		floorButtons[n-1][upordown] = false;
	}

	inline void turnOnElevatorButton(int n, int k){
		elevators[k-1].elevatorButtons[n-1] = true;
	}

	inline void turnOffElevatorButton(int n, int k){
		elevators[k-1].elevatorButtons[n-1] = false;
	}

	//methods for retrieving distribution
	inline float getProbElev(int n,int k){
		//prob a person in elevator k gets off at floor n
		return ((float)elevators[k-1].elevatorDistribution[n-1]/normalise);
	}

	inline float getProbFloUp(int n){
		//prob that a person on floor n wants to go up
		return ((float)floorDistribution[n-1][1]/normalise);
	}

	inline float getProbFloDown(int n){
		//prob that a perosn on floor n wants to go down
		return ((float)floorDistribution[n-1][0]/normalise);
	}

	// methods for changing distributions
	inline void setProbElev(int k, int n, int newProb){
		elevators[k-1].elevatorDistribution[n-1] = newProb;
	}

	inline void setProbFloUp(int n, int newProb){
		floorDistribution[n-1][1] = newProb;
	}

	inline void setProbFloDown(int n, int newProb){
		floorDistribution[n-1][0] = newProb;
	}
		
};

#endif