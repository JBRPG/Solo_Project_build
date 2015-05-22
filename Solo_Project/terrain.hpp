#pragma once

#include <SFML\Graphics.hpp>

#include "entity.hpp"

class Terrain : public Entity{

private:


public:

	Terrain();

	Terrain(const sf::Texture&, int, float, bool);

	Terrain(const sf::Texture&, sf::IntRect&,
		    int, float, bool);


	void collideWith(Entity&);
	

	// empty functions
	void updateMovement(Movement&) {};
	void updateWeapon(Weapon&) {};

};