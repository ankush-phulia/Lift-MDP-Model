#ifndef STATE_H
#define STATE_H
#include <bitset>
#include <vector>

using namespace std;


class state{

public:
	bitset<200> mystate;
	vector<state> links;
	int P, Q, R;

	state(state new_state);
	void create_state(vector<string> action, state current_state);
	void perform_action(string action, state current_state);

	//retrieval methods
	int get_destination(int n);
	int get_location(int n);
	int get_waiting_time(int n);
	bool does_person_exist(int n);
	bool floor_button_pressed(int n);
	bool elevator_button_pressed(int n, int k);
	bool doors_opened(int k);
	int get_direction(int k);
	int get_floor(int k);

	//update methods
	void update_floor(int n, int k);
	void elevator_moves(int dir, int k);
	void elevator_doors(bool open, int k);
	void lift_button(int n, int k, bool pressed);
	void floor_button(int n, bool pressed);
	void add_person(int n, int floor);
	void delete_person(int n);
	void person_moves(int n, int loc);
	void updsonate_per_time(int n);
	void update_person_destination(int n, int floor);	
};

#endif