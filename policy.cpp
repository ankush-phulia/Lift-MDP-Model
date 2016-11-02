#include "policy.h"
#include "simulate.h"

action getActionforState(Policy &currentPolicy, int currentState){
	//not sure about correctness
	return currentPolicy[currentState];
}

void updatePolicy(action newAction, Policy &currentPolicy, int currentState){
	currentPolicy[currentState] = newAction;
}

void initialisr(int state){

}

float updateActionValue(int state,int depth, simulate &simulator){
	if (depth == 0){
		return simulator.total_cost
	}
	else{
		action nextAc = Policy[state];
		simulator.performAction(nextAc);
		float cost = updateActionValue(simulator.return_midstate(),depth-1,simulator);
		int n = ++PolicyValues[state].visited;
		if (PolicyValues[state].Qval.empty()){
			PolicyValues[state].Qval.insert({nextAc,0.0});
		}
		PolicyValues[state].Qval[nextAc] = (1-1.0/n)*(PolicyValues[state].Qval[nextAc]) + (1.0/n)*(cost);
	}
}