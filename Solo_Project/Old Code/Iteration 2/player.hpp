#pragma once

#include "SFML\Graphics.hpp"

#include "textureManager.hpp"


/* Right now we are working with only one player*/

class Player{


public:
	
	Player(const sf::Texture& tex);
	~Player();

	
	TextureManager texmgr;

	void shoot();
	void move(bool*);
	void loseLife();
	int getLives();
	double getSpeed();
	sf::Sprite getSprite();


private:

	sf::Sprite playerSprite;

	int lives;
    int firepower;
	double speed;
};