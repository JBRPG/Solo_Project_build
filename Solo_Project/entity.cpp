#include "entity.hpp"



// Constructors

/*

// for enemy creation
Entity(SceneGame* scene, std::string tex, int hp, float speed, bool invincible,
	sf::Vector2f pos, Weapon* weapon, Movement* movement) :
	myScene(scene), health(hp), speed(speed), invincible(invincible), myWeapon(weapon), myMovement(movement)
{
	this->myScene->game->texmgr.getRef(tex);
	this->setPosition(pos);
};

// for player creation
Entity(SceneGame* scene, std::string tex, int hp, float speed, bool invincible,
	sf::Vector2f pos, Weapon* weapon) :
	myScene(scene), health(hp), speed(speed), invincible(invincible), myWeapon(weapon)
{
	this->myScene->game->texmgr.getRef(tex);
	this->setPosition(pos);
};

// for bullet
Entity(SceneGame* scene, std::string tex, int hp, float speed, bool invincible,
	sf::Vector2f pos) :
	myScene(scene), health(hp), speed(speed), invincible(invincible)
{
	this->myScene->game->texmgr.getRef(tex);
	this->setPosition(pos);
};

//*/


// Will add more code soon
void Entity::update(float dt){
	++ticks;
}

Entity::~Entity(){
	if (myMovement != nullptr){
		delete myMovement;
		myMovement = nullptr;
	}

	if (myWeapon != nullptr){
		delete myWeapon;
		myWeapon = nullptr;
	}
}
