#pragma once

#include <SFML\Graphics.hpp>

#include "weapon.hpp"
#include "movement.hpp"


// forward declare

class EnemyTemplate;

/*
   Spawner will create enemies and/or power ups

   For spawning enemies, they will recieve one weapon and one movement type

   for powerups, only one will spawn and the spawner would disappear afterwards

   The spawner will create entities through the following:

   Spawn gap: the amount of time to wait between making enemies, in frame ticks
   Spawn limit: the amount of enemies to make before being removed
   


   */

class Spawner{
private:

	int spawnGapTime;
	int spawnGapSet;

	// If you plan to spawn finite enemies, then use constructor
	int spawnLimit = -1;

	EnemyTemplate* enemyData;
	Weapon* givenWeapon;
	Movement* givenMovement;

	// make sure to give the spawner the coordinates for window in pixels
	sf::Vector2i windowCoords;


	int ticks;

public:

	Spawner(Weapon*, Movement*, EnemyTemplate*, std::vector <int>);
	Spawner(Weapon*, Movement*, EnemyTemplate*, std::vector <int>, sf::Vector2i);


	void update();
	void spawn_enemy();
	void spawn_count();

	int getSpawnLimit() { return spawnLimit; };

	void setSpawnLocation(sf::Vector2f);

	sf::Vector2i getWindowCoords();

	~Spawner(){
		delete givenWeapon;
		delete givenMovement;
		delete enemyData;
	};


};