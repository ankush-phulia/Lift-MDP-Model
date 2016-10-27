#ifndef BASIC_H
#define BASIC_H

uint32_t elevator;     //32 bits
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
uint64_t people1; //8 ppl
//0-7 person0
//0 - present in system
//1-4 - waiting time
//5-7 - floor which he wants to go/at????
uint64_t people2; //8 ppl

//methods for elevators
inline int getFloor(int k){
	//elevator 0 or 1
	return (elevator>>(3*n))&7;
}
inline bool isOpen(int k){
	return (elevator&(1<<(28+n)))==1;
}
inline int getDirection(int k){
	return (elevator>>(27+n))&3;
}
inline bool elevatorButtonPressed(int k, int n){
	//k = 0,1 ; n =0,1,2,3,4
	return (elevator&(1<<(14+8*k+n)))==1;
}

inline bool floorButtonPressed(int n){
	//????
}

//methods for persons
bool doesPersonExist(int n){
	//n: 0-15
	return (n<8)?(people1&(1<<(8*n))):(people2&(1<<(8*n)));
}
int waitingTime(int n){
	return (n<8)?(people1&()):(people2&());
}
inline int getLocation (int n){
	return (n<8)?(people1&()):(people&());
}

int dummy_func(); //dyummy func to big to be fitted inline where its called