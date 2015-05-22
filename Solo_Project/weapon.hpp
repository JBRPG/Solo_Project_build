#pragma once

#include <SFML/Graphics.hpp>

#include <functional>
#include <iostream>

#include "bullet.hpp"
#include "sceneGame.hpp"
#include "input.hpp"
#include "textureManager.hpp"

#include "entity.hpp"
#include "player.hpp"
#include "enemy.hpp"

// Forward declare
class BulletTemplate;


/*
  Weapon consist of a collection of bullets

  There will be two types of Weapon objects
   - Player variant will be pressed by the shoot button
   - Enemy variant will shoot bullets after a set period of time

  Bullets can be all shot at the same time, or in a specified sequence based on time (in ticks)


*/

class Weapon{
private:

	// Delay types
	int shootCooldownTime; // general delay time until shooting
	int shootCooldownSet;  

	int rapidWait; // How many shots per second
	int rapidWaitSet;
	int rapidDuration; // rapid fire time until cooldown
	int rapidDurationSet;

	// Enemy only
	int sequenceDelayTime; // delay time between individual bullets 
	int sequenceDelaySet;
	int sequence_idx;

	// Player only
	int holdDurationTime; // player does not let go, shot enter cooldown
	int holdDurationSet;  // otherwise, weapon ready to fire when pressed
	int shot_limit = 4;
	int shot_count;

	int spawnTime;

	std::string keyword; // Determines behavior of weapon

	std::vector< BulletTemplate* > bullet_list;
	bool enemydidShoot;


	// functions
	void rapidFire(Entity&);
	void sequenceFire(Entity&);

	// Functions for shooting bullets

	void shootBullets(Entity&); // Shoot all bullets at once
	void shootBullet(Entity&, BulletTemplate&); // shoot one bullet each in a sequence

public:

	// Constructors

	Weapon();
	Weapon(std::vector <BulletTemplate*>, std::string, int, std::vector<int>);
	Weapon(std::vector <BulletTemplate*>, std::string, int);

	// copy constructor
	Weapon(const Weapon&);

	// assignment operator
	Weapon& operator=(const Weapon&);

	// destructor
	~Weapon();

	// Functions

	void update(Entity&);

	Weapon* replaceWeapon(Weapon*);

	/*
	   Look up the name of the specified shot type to
	   call the specified functions
	*/
	void lookupShoot(Entity&, std::string);


	/* The required parameters for the specified functions
	   1 - Entity&: Use it to produce bullets and manage cooldown
	*/
	void rapidEnemy(Entity&);
	void rapidPlayer(Entity&);
	void holdPlayer(Entity&);
	void sequenceEnemy(Entity&);
	void singleShot(Entity&);

};

class WeaponTemplate{
private:
	std::vector<BulletTemplate*> bullets;
	std::string name;
	int delay;
	std::vector<int> params;

public:
	WeaponTemplate(std::vector <BulletTemplate*> bullets, std::string name, int delay, std::vector<int> param):
		bullets(bullets), name(name), delay(delay), params(param)
	{};
	WeaponTemplate(std::vector <BulletTemplate*> bullets, std::string name, int delay) :
		bullets(bullets), name(name), delay(delay)
	{};


};