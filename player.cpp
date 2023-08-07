# include<iostream>
# include<stdio.h>
# include"player.hpp"
using namespace std;
Player::Player(){x = 1;
 old_x = 1; 
 y = 4; 
 old_y = 4;}		
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
	old_x = x;
	if (key == 1){
		x--;
	}
	else{
		x++;
	}
	
}
void Player::set_y(int key){
	old_y = y;
	if (key == 2){
		y--;
	}
	
	else{
		y++;
	}

}




