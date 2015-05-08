#pragma once

#include "entity.hpp"
#include "weapon.hpp"

class Pickup : public Entity{
private:

public:
	Pickup(SceneGame* scene, std::string tex, sf::Vector2f pos, Weapon* weapon) :
	Entity(scene, tex, pos, weapon){

	};

	void collideWith(Entity&);
	Weapon* giveWeapon();


	// empty functions
	void updateMovement(Movement&) {};
	void updateWeapon(Weapon&) {};

};