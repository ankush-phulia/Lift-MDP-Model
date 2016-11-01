#include "policy.h"


state performAction(action ac, state &currentState){

}

void genAllActions(std::vector<action> &actions,state &currentState){
	
}

action getActionforState(Policy &currentPolicy, int currentState){
	//not sure about correctness
	return currentPolicy[currentState];
}

void updatePolicy(action newAction, Policy &currentPolicy, int currentState){
	currentPolicy[currentState] = newAction;
}