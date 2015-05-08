#pragma once

#include <SFML\Graphics.hpp>

#include "entity.hpp"
//#include "player.hpp"
//#include "bullet.hpp"
//#include "enemy.hpp"

// forward declare
class Player;
class Enemy;
class Bullet;


class Terrain : public Entity{

private:

	// From entity
	int hp;
	float speed;
	bool invincibility;


public:

	Terrain(const sf::Texture& tex, int hp, float speed, bool invincible) :
		hp{hp},
		speed{speed},
		invincibility{invincible},
		Entity(tex, hp, speed, invincible){};

	Terrain(const sf::Texture& tex, sf::IntRect& rect,
		    int hp, float speed, bool invincible) :
		hp{ hp },
		speed{ speed },
		invincibility{ invincible },
		Entity(tex, rect, hp, speed, invincible){};

	void collideWith(Player*);
	void collideWith(Bullet*);
	void collideWith(Enemy*);


};