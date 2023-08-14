# include<iostream>
# include<stdio.h>
using namespace std;
class Player{
	int x;
	int old_x;
	int y;
	int old_y;
	int penultimate_blow;
	int number_of_steps;
	public:
		Player();
		int get_old_x();
		int get_old_y();
		int get_x();
		int get_y();
		void set_x(int key);
		void set_y(int key);
		void set_old(int key);
		void reinitialise_process_set(int i, int j);
		void reinitialize();
};

