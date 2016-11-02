#ifndef POLICY_H
#define POLICY_H
// #include "state.h"
#include <unordered_map>

int max_depth = 10;
int max_iter = 10;
struct node{
	std::unordered_map<action,float> Qval;
	int visited;
}

typedef uint8_t action;
//some encoding for action
typedef std::unordered_map<int,action> Policy;
typedef std::unordered_map<int, node> PolicyValues;
//state->action is the policy --store everything??!!

// state performAction(uint8_t ac, state &currentState);
//do we really need this??!!!
// void genAllActions(std::vector<action> &actions,state &currentState);
action getActionforState(Policy &currentPolicy, state &currentState);
void updatePolicy(action newAction, Policy &currentPolicy, state &currentState);

#endif