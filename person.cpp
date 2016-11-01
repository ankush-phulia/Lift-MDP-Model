#include "person.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

person::person(int id, int N, float q, float r) {
	srand(time(0));
	this->id = id;
	this->elevator_number = -1;
	int random = rand()%100 + 1;
	if(random < 100*q)
		this->start = 1;
	else
		this->start = rand()%(N-1) + 2;
	if(start != 1) {
		random = rand()%100 + 1;
		if(random < 100*r)
			this->dest = 1;
		else {
			random = rand()%(N-2) + 1;
			int floor, count = 0;
			for(int i = 2; i <= N; i++) {
				if(i != this->start)
					count++;
				if(random == count) {
					this->dest = i;
					break;
				}
			}
		}	
	}
	else {
		this->dest = rand()%(N-1) + 2;
	}
	if(this->start > this->dest) 
		direction = 0;
	else
		direction = 1;
}