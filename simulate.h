#ifndef SIMULATE_H
#define SIMULATE_H
#include "state.h"
#include "person.h"
#include <stdlib.h>
#include <vector>
#include <iostream>

using namespace std;

// CONSTANTS
const int WAIT_TIME_COST_FACTOR = 2;
const int UP_DOWN_COST_FACTOR = 1;

class simulate {
public:
	state sim_state;
	int N;
	int K;
	float p;
	float q;
	float r;	
	int t;
	int total_cost;
    int total_people_served;
    vector<person> people_in_sys;

    // CONSTRUCTOR
	simulate(int N, int K, float p, float q, float r, int t);

	void perform_action(int);
	state return_midstate();
	void print_state();
	void remove_person(int );
	void add_person(int , int , float , float );
};

#endif