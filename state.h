#ifndef STATE_H
#define STATE_H
#include <stdlib.h>
#include <iostream>

// using namespace std;

class state{

public:
	uint32_t elevator;
	//0-2: elevator 0 position
	//3-5: elevator 1 position
	//6-15 : buttons on floors //down and up
	//16-20: elevator 0 buttons
	//21-25: elevator 1 buttons
	//26-27: e1 direction
	//28-29: e2 direction
	//30: e1 open = 1/closed = 0
	//31: e2 open = 1/closed =0
	//etc
	int e_dist[5][2];
	// prob distr. of ppl wanting to get off at each floor - 12 bits per floor
	int floor_dist[5][2];
	// prob distr. of ppl wanting to go up or down on each floor - 12 bits per direction
	int normalise;// normalising constant - 12 bit number

	state();//constructor

	//methods for retrieving elevators
	inline int getFloor(int k){
		//elevator 1 or 2
		return (elevator>>(3*(k-1)))&7;
	}
	inline bool isOpen(int k){
		return ((elevator>>(30+k-1))&1)==1;
	}
	inline int getDirection(int k){
		return (elevator>>(26+2*k-2))&3;
	}
	inline bool elevatorButtonPressed(int k, int n){
		//k = 1,2 ; n = 1,2,3,4,5
		return ((elevator>>(16+5*(k-1)+n-1))&1)==1;
	}
	inline bool floorButtonPressed(int n, int upordown){
		//make upordown bool??!!
		//careful when using button for floor 1 or 5, always give upordown as 0 or false
		return ((elevator>>(6+2*(n-1)+upordown))&1)==1;
	}

	//methods for changing elevators
	void setFloor(int k, int n);

	inline void toggleDoor(int k){
		//open-> close and close->open
		elevator ^= (1<<(30+k-1));
	}

	inline void openDoor(int k){
		elevator |= (1<<(30+k-1));
	}

	inline void closeDoor(int k){
		elevator |= (1<<(30+k-1));
		elevator ^= (1<<(30+k-1));
	}

	//10 for static, 01 for up, 00 for down
	void setDirection(int k, int dir);

	// inline void toggleFloorButton(int n, int upordown){
	// 	//make upordown bool??!!
	// 	elevator ^= (1<<(6+n+upordown));
	// }

	inline void turnOnFloorButton(int n, int upordown){
		elevator |= 1<<(6+2*(n-1)+upordown);
	}

	inline void turnOffFloorButton(int n, int upordown){
		elevator |= 1<<(6+2*(n-1)+upordown);
		elevator ^= 1<<(6+2*(n-1)+upordown);
	}

	// inline void toggleElevatorButton(int n, int k){
	// 	elevator ^= (1<<(16+8*(k-1)+n-1));
	// }

	inline void turnOnElevatorButton(int n, int k){
		elevator |= (1<<(16+5*(k-1)+n-1));
	}

	inline void turnOffElevatorButton(int n, int k){
		elevator |= (1<<(16+5*(k-1)+n-1));
		elevator ^= (1<<(16+5*(k-1)+n-1));		
	}


	//methods for retrieving distribution
	inline float getProbElev(int n,int k){
		//prob a person in elevator k gets off at floor n
		return e_dist[n-1][k-1]/(float)normalise;
	}

	inline float getProbFloUp(int n){
		//prob that a person on floor n wants to go up
		return floor_dist[n-1][1]/(float)normalise;
	}

	inline float getProbFloDown(int n){
		//prob that a perosn on floor n wants to go down
		return floor_dist[n-1][0]/(float)normalise;
	}

	// methods for changing distributions
	void setProbElev(int k, int n, int newProb);
	void setProbFloUp(int n, int newProb);
	void setProbFloDown(int n, int newProb);
		
};

#endif