#pragma once

#include <SFML\Graphics.hpp>

#include <iostream>

#include "entity.hpp"
#include "bullet.hpp"
#include "enemy.hpp"

#include "input.hpp"
#include "sceneGame.hpp"

// forward declaration
//class SceneGame;

class Player : public Entity{

private:

	unsigned int score = 0;
	float spawnTime = 5.0f;
	unsigned int lives;


public:

	Player(const sf::Texture& tex, int hp, float speed,
		bool invincibility, int lives) :
		lives(lives),
		Entity(tex, hp, speed, invincibility)
	{};

	Player(const sf::Texture& tex, const sf::IntRect& rect, int hp,
		  float speed, bool invincibility, int lives) :
		lives(lives),
		Entity(tex, rect, hp, speed, invincibility)
	{};

	Player(SceneGame* scene, std::string tex, int hp, float speed, bool invincibility,
		int lives, sf::Vector2f pos, Weapon* weapon) :
		lives(lives),
		Entity(scene, tex, hp, speed, invincibility, pos, weapon)
	{};

	void update(float dt);


	int getTicks() { return ticks; };


	void collideWith(Entity&);
	
	void movePlayer();
	void shootPlayer(float dt);
	void checkHealthPlayer();


	// empty functions

	void updateMovement(Movement&) {};

	// Leave it empty for now, will implement it later
	void updateWeapon(Weapon&) {};


};