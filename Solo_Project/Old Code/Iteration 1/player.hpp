#pragma once

#include "SFML\Graphics.hpp"

#include "state_game.hpp"
// #include "game.hpp"

// class Game; // forward declare

/* Right now we are working with only one player*/

class Player{


public:
	
	Player();
	~Player();

	sf::Sprite playerSprite;
	TextureManager texmgr;

	void shoot();
	void move(bool*);
	void loseLife();
	void getLife();
	double getSpeed();


private:

	int lives;
    int firepower;
	double speed;
};