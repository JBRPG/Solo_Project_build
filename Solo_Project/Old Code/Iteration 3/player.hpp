#pragma once

#include "SFML\Graphics.hpp"

#include "textureManager.hpp"
#include "entity.hpp"


/* Right now we are working with only one player*/

class Player{


public:
	
	Player();

	
	TextureManager texmgr;

	void shoot();
	void move(bool*);
	void loseLife();
	int getLives() { return lives; };
	void setLives(int lives) { this->lives = lives; };
	double getSpeed() { return speed; };
	void setSpeed(double speed) { this->speed = speed; };
	Entity* getEntity() { return playerEntity; };
	void setEntity(Entity* e) { this->playerEntity = e; };


private:

	//sf::Sprite playerSprite;
	Entity*    playerEntity;

	int lives;
    int firepower;
	double speed;
};