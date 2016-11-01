#ifndef PERSON_H
#define PERSON_H

class person {
public:
	int id;
	int elevator_number;
	int start;
	int dest;
	int direction;

	// CONSTRUCTOR
	person(int id, int N, float q, float r); 
};

#endif