#include "player.hpp"

#include <iostream> // for debug purposes

// player constructor will require texture as parameter
Player::Player(){
	


	this->lives = 3;
	this->speed = 5;
	this->firepower = 1;



}

void Player::move(bool* dirButtons){
	// move ship based on arrow keys pressed

	double vel_x = 0;
	double vel_y = 0;

	vel_x = -speed * double(dirButtons[0]); // left
	vel_x += speed * double(dirButtons[2]); // right

	vel_y = -speed * double(dirButtons[1]); // up
	vel_y += speed * double(dirButtons[3]); // down

	double sqrt2 = 1.41; // square root of 2

	if ((dirButtons[0] || dirButtons[2]) && (dirButtons[1] || dirButtons[3])){
		vel_x /= sqrt2;
		vel_y /= sqrt2;
	}
	if (dirButtons[0])
	{
		this->playerEntity->setPosition(sf::Vector2f(400, 0));
		this->playerEntity->getSprite().move(400, 0);// (sf::Vector2f(0, 0));
	}
	this->playerEntity->getSprite().move(5, vel_y);
}

void Player::loseLife(){
	--this->lives;
}
