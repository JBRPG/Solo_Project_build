#pragma once

#include <string>
#include <vector>

#include "entity.hpp"
#include "player.hpp"
#include "bullet.hpp"
#include "terrain.hpp"
#include "explosion.hpp"

#include "sceneGame.hpp"

// Forward declarations
//class Player;

class Enemy : public Entity{

private:

	sf::Vector2f spawnVertex;


	// Group related variables
	bool withGroup;
	int groupID; // the enemy is solo if value is set at 0

	// functions

	void destroyEnemy(); // make the enemy explode and disappear

public:



	Enemy(const sf::Texture& tex, int hp, float speed, bool invincible) :
		Entity(tex, hp, speed, invincible), withGroup{ false }, groupID{ 0 },
		spawnVertex{ this->getPosition() }
	{};
	Enemy(const sf::Texture& tex, const sf::IntRect& rect,
		int hp, float speed, bool invincible) :
		Entity(tex, rect, hp, speed, invincible), withGroup{ false }, groupID{ 0 },
		spawnVertex{ this->getPosition() }
	{};
	Enemy(SceneGame* scene, std::string tex, int hp, float speed, bool invincible,
		sf::Vector2f pos, Weapon* weapon, Movement* movement) :
		spawnVertex(pos), withGroup{ false }, groupID{ 0 },
		Entity(scene, tex, hp, speed, invincible, pos, weapon, movement)
	{};

	Enemy();

	void setWithGroup(bool group) { this->withGroup = group; };
	bool getWithGroup() { return this->withGroup; };

	void setGroupID(int id) { this->groupID = id; };
	int getGroupID() { return this->groupID; };


	void collideWith(Entity&);
	

	void update(float dt);
	int getTicks() { return ticks; };


	void updateMovement(Movement&);
	void updateWeapon(Weapon&);


};

class EnemyTemplate {
private:

	SceneGame* myScene;
	std::string strTex;
	int health;
	float speed;
	bool invincible;
	sf::Vector2f spawnPos;


public:

	EnemyTemplate(SceneGame*, std::string, int, float, bool, sf::Vector2f);

	std::string getTex() { return strTex; };
	int  getHP() { return health; };
	float  getSpeed() { return speed; };
	bool  getInvincible() { return invincible; };
	sf::Vector2f  getSpawnPos() { return spawnPos; };
	void setSpawnPos(sf::Vector2f vertex) { spawnPos = vertex; };
	SceneGame* getScene() { return myScene; };

	~EnemyTemplate();

};