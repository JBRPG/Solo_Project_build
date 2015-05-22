#pragma once

#include "enemy.hpp"

// forward declare

class Movement;
class Weapon;

class EnemyTerrain : public Enemy{
private:

	// variables

	float gravity;
	float fallSpeed; // add gravity value for every tick

	bool turn_edge;
	bool on_terrain;
	bool fall_dir; // affected by gravity value, used for landing on terrain at top or bottom

	// functions

protected:



public:

	// constructors
	EnemyTerrain(SceneGame*, std::string, int, float, bool,
		sf::Vector2f, Weapon*, Movement*,
		float, bool);


	// functions

	void setGravity(float);
	float getGravity();

	void setFallSpeed(float);
	float getFallSpeed();

	void setTurnEdge(bool);
	bool getTurnEdge();

	void setOnTerrain(bool);
	bool getOnTerrain();




	void collideWith(Entity&);

	void update(float dt);
	int getTicks();

	void updateMovemnt(Movement&);
	void updateWeapon(Weapon&);


};