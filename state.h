#ifndef STATE_H
#define STATE_H
#include <bitset>
#include <vector>

using namespace std;


class state{

public:
	uint32_t elevator;
	//0-2: elevator 0 position
	//3-5: elevator 1 position
	//6 : button on 1st floor
	//7-12: buttons on 2nd - 4th floor
	//13: button on 5th floor
	//14-18: elevator 0 buttons
	//19-23: elevator 1 buttons
	//24-25: e0 direction
	//26-27: e1 direction
	//28: e0 open = 1/closed
	//29: e1 open = 1/closed
	//etc
	uint64_t e0_dist;
	uint64_t e1_dist;
	//prob distr. of ppl wanting to get off at each floor - 12 bits per floor
	uint32_t *floor_dist;
	//prob distr. of ppl wanting to go up or down on each floor - 12 bits per direction
	int normalise;// normalising constant - 12 bit number

	state(int n);//constructor

	//methods for retrieving elevators
	inline int getFloor(int k){
		//elevator 0 or 1
		return (elevator>>(3*k))&7;
	}
	inline bool isOpen(int k){
		return (elevator&(1<<(28+k)))==1;
	}
	inline int getDirection(int k){
		return (elevator>>(27+k))&3;
	}
	inline bool elevatorButtonPressed(int k, int n){
		//k = 0,1 ; n = 0,1,2,3,4
		return (elevator&(1<<(14+8*k+n)))==1;
	}
	inline bool floorButtonPressed(int n, int upordown){
		//make upordown bool??!!
		//careful when using button for floor 1 or 5, always give upordown as 0 or false
		return (elevator&(1<<(6+n+upordown)))==1;
	}

	//methods for changing elevators
	void setFloor(int k, int n);

	inline void toggleDoor(int k){
		//open-> close and close->open
		elevator ^= (1<<(28+k));
	}

	//00 for static, 01 for up, 10 for down
	inline void setDirection(int k, int dir);

	inline void toggleFloorButton(int n, int upordown){
		//make upordown bool??!!
		//careful when using button for floor 1 or 5, always give upordown as 0 or false
		elevator ^= (1<<(6+n+upordown));
	}

	inline void toggleElevatorButton(int n, int k){
		elevator ^= (1<<(14+8*k+n));
	}

	//methods for retrieving distribution
	inline float getProbElev(int n,int k){
		//prob a person in elevator k gets off at floor n
		return (k==0)?(((e0_dist>>(12*n))&(4095))/normalise):(((e1_dist>>(12*n))&(4095))/normalise);
	}

	inline float getProbFloUp(int n){
		//prob that a person on floor n wants to go up
		return (floor_dist[n]&(4095))/normalise;
	}

	inline float getProbFloDown(int n){
		//prob that a perosn on floor n wants to go down
		return ((floor_dist[n]>>12)&(4095))/normalise;
	}

	//methods for changing distributions
	void setProbElev(int k, int n, int newProb);
	void setProbFloUp(int n, int newProb);
	void setProbFloDown(int n, int newProb);
		
};

#endif