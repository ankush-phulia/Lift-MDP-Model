#include "policy.h"


void policy::doAction(action ac, state &currentState){

}

void policy::undoAction(action ac, state &currentState){

}

void policy::genAllActions(std::vector<action> &actions,state &currentState){
	
}

action policy::getActionforState(policy &currentPolicy, state &currentState){
	//not sure about correctness
	return currentPolicy[currentState];
}

void policy::updatePolicy(action newAction, policy &currentPolicy, state &currentState){
	currentPolicy[currentState] = newAction;
}