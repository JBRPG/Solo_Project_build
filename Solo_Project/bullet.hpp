#pragma once

#include <SFML\Graphics.hpp>

#include "entity.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "terrain.hpp"

#include "sceneGame.hpp"

// forward declaration

class Weapon;


class Bullet : public Entity{

private:
	// From entity
	//int hp;
	//float speed;
	//bool invincibility;

	bool enemyShot; // if false, then player shot bullet
	float rotation;
	Weapon* firedFrom;


public:

	// constructors

	Bullet(const sf::Texture& tex, int hp, float speed, bool invincibility,
		bool enemyShot, float rot) :
		rotation(rot), enemyShot(enemyShot),
		Entity(tex, hp, speed, invincibility)
	{};

	Bullet(const sf::Texture& tex, const sf::IntRect& rect, int hp, float speed, bool invincibility,
		bool enemyShot, float rot) :
		rotation(rot), enemyShot(enemyShot),
		Entity(tex, rect, hp, speed, invincibility)
	{};

	Bullet(SceneGame*, std::string, int, float, bool, sf::Vector2f);
	Bullet(SceneGame*, std::string, int, float, bool, sf::Vector2f, bool, float);

	~Bullet();


	// member functions

	void setEnemyShot(bool shot) { enemyShot = shot; };
	bool getEnemyShot() { return enemyShot; };


	void collideWith(Entity&);
	void update (float dt);
	int getTicks() { return ticks; };

	Weapon* getFiredFrom();
	void setFiredFrom(Weapon*);

	// I want the bullet to have dynamic movement
	// and will be implemented later

	// For now, we leave the updateMovement empty
	void updateMovement(Movement&) {};

	// We will leave this function empty
	void updateWeapon(Weapon&) {};

	
};

/*
  We will have a class that only uses simple bullet data to make
  bullet object initalization easier with little to no hardcoding
*/

class BulletTemplate{
private:

	std::string texString;
	int hp;
	float speed;
	bool invincibility;
	float rotation;



public:

	// Constructor layout will be the same as the bullet,
	// but tex will have string instead of sf::Texture
	BulletTemplate(std::string tex, int _hp, float _speed, bool invinc, float rot):
	texString(tex), hp(_hp), speed(_speed), invincibility(invinc), rotation(rot)
	{};
	BulletTemplate();

	// copy constructor
	BulletTemplate(const BulletTemplate&);

	// overloaded assignment operator
	BulletTemplate& operator=(const BulletTemplate&);

	// destructor
	~BulletTemplate();

	std::string getTex() { return texString; };
	int getHP() { return hp; };
	float getSpeed() { return speed; };
	bool getInvincible() { return invincibility; };
	float getRotation() { return rotation; };


};