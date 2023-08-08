# include<iostream>
# include<stdio.h>
# include"player.hpp"
using namespace std;
Player::Player(){x = 1;
 old_x = 1; 
 y = 4; 
 old_y = 4;
 penultimate_blow = 5;
 number_of_steps = 0;}		
int Player::get_old_x(){
	return old_x;
}
int Player::get_old_y(){
	return old_y;
}
int Player::get_x(){
	return x;
}
int Player::get_y(){
	return y;
}
void Player::set_x(int key){
	set_old(key);
	if (key == 1){
		x--;
	}
	else{
		x++;
	}
	number_of_steps++;
	penultimate_blow = key;
}
void Player::set_y(int key){
	set_old(key);
	if (key == 2){
		y--;
	}
	
	else{
		y++;
	}
	number_of_steps++;
	penultimate_blow = key;
}
void Player::set_old(int key){
	if (number_of_steps){
		if ((penultimate_blow == 1 or penultimate_blow == 3) and (key == 1 or key == 3)){
			old_x = x;
		}
		else if ((penultimate_blow == 1 or penultimate_blow == 3) and (key == 2 or key == 4)){
			old_x = x;
		}
		else if ((penultimate_blow == 2 or penultimate_blow == 4) and (key == 2 or key == 4)){
			old_y = y;
		}
		else if ((penultimate_blow == 2 or penultimate_blow == 4) and (key == 1 or key == 3)){
			old_y = y;
		}
	}
}




