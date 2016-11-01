#ifndef POLICY_H
#define POLICY_H
#include "state.h"
#include <unordered_map>

typedef int action;
//some encoding for action
typedef std::unordered_map<int,action> Policy;
//state->action is the policy --store everything??!!

void performAction(action ac, state &currentState);
//do we really need this??!!!
void genAllActions(std::vector<action> &actions,state &currentState);
action getActionforState(Policy &currentPolicy, state &currentState);
void updatePolicy(action newAction, Policy &currentPolicy, state &currentState);

#endif