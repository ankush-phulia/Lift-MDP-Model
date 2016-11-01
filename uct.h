#ifndef UCT_H
#define UCT_H
#include "policy.h"
#include "math.h"

int UCTK = 1;
int max_depth = 10;
int max_iter = 10;

struct node{
	state elevatorState;
	uint16_t visited;
	uint16_t cost; //-ve for maximisation
	action move;
	*node parent;
	std::vector<*node> children;
};

node createNode(state &s, action m){
	node n;
	n.elevatorState = s;
	n.visited = 0;
	n.cost = 0;
	n.move = m;
}

// action bestAction(node &n){
// 	action bestAc;
// 	float bestR = -10000000000;
// 	for (node child:n.children){
// 		float curr = child.cost/child.visited;
// 		if (curr > bestAc){
// 			bestAc = child.move;
// 			bestR = curr;
// 		}
// 	}
// 	return bestAc;
// }

void UCTExpand(node &n){
	std::vector<action> actions;
	genAllActions(actions,n.elevatorState);

	for (action act:actions){
		state newState = performAction(act,n.elevatorState);
		node child = createNode(newState, act);
		node *temp = &n;
		child.parent = n;
		*temp = &child;
		n.children.push_back(temp);
	}
}

node UCTSelect(node &n,int c){
	float best = -1000000000;
	node bestCh;
	for (node child:n.children){
		float curr;
		if (n.visited>0){
			curr = next.cost/next.visited + c*(sqrt(2*(log(n.visited))/next.visited));	
		}
		else{
			bestCh = child;
			break;
		}
		if (curr > best){
			best = curr;
			bestCh = child;
		}
	}
	return bestCh;
}

node treePolicy(node &n){
	node current = n;
	if (!current.children.empty()){
		while (!current.children.empty()){
			current = UCTSelect(current,UCTK);
		}
		UCTExpand(current);
		return current;		
	}
	else{
		UCTExpand(n);
		return n.children[0];
	}

}

uint16_t defaultPolicy(node &n, int d){
	node current = n;
	while (d<max_depth){
		current = simulate(randomAction(current));
		d++;
	}
	return current.cost/current.visited;
}

void backUp(node n, int val){
	while (n!=NULL){
		n.visited++;
		n.cost = n.cost + val;
		n = n.parent;
	}
}

action UCTSearch(node &n,int depth){
	int iter = 0;
	while (iter < max_iter){
		node next = treePolicy(n);
		uint16_t reward = defaultPolicy(0,next);
		backUp(next,reward);
		iter++;
	}
	return (UCTSelect(n,0).move);
}


#endif