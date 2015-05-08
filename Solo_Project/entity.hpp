#pragma once

#include <SFML\Graphics.hpp>

#include <string>
#include <vector>

#include "textureManager.hpp"

//#include "sceneGame.hpp"


/*
   Entity inherits all public and protected data and functions from
   Sprite class. Sprite inherits from both Drawable and Transformable.

   Because Entity uses drawing, transformation, and sprite functions,
   it takes only one inheritence from Sprite to reduce coding burdens.
   
*/

// forward declare
class SceneGame;

class Movement;

class Weapon;


class Entity : public sf::Sprite{

private:


protected:

	int health;
	float speed;
	sf::Time spawntime;
	bool invincible;

	SceneGame *myScene;
	Movement *myMovement;
	Weapon *myWeapon;

	std::vector <float> moveArgs;

	int ticks = 0; // We are counting up frame by frame for behavior consistency 




public:

	/*
	   First let's make the constructors that take only
	   Sprite's parameters
	*/

	Entity(const sf::Texture& tex) :Sprite(tex),
		health{ 1 }, speed{ 0 }, invincible{ true }
	{
		setOrigin(this->getGlobalBounds().width/2, this->getGlobalBounds().height/2);
	};
	Entity(const sf::Texture& tex, const sf::IntRect& rect) :
		Sprite(tex, rect),
		health{ 1 }, speed{ 0 }, invincible{ true }
	{
		setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);
	};

	/*
	   Then we make constructors that have their own parameters
	   AFTER placing Sprite's parameters first
	*/
	Entity(const sf::Texture& tex, int hp, float speed, bool invincible) :
		Sprite(tex),
		health( hp ), speed( speed ), invincible(invincible)
	{
		setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);
	};
	Entity(const sf::Texture& tex, const sf::IntRect& rect,
		int hp, float speed, bool invincible) :
		Sprite(tex, rect),
		health(hp), speed(speed), invincible(invincible)
	{
		setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);
	};

	// Other constructors

	//*
	// for enemy creation
	Entity(SceneGame* scene, std::string tex, int hp, float speed, bool invincible,
		sf::Vector2f pos, Weapon* weapon, Movement* movement) :
		myScene(scene), health(hp), speed(speed), invincible(invincible), myWeapon(weapon), myMovement(movement),
		Sprite(TextureManager::instance()->getRef(tex))
	{
		setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);
		this->setPosition(pos);
	};

	// for player creation
	Entity(SceneGame* scene, std::string tex, int hp, float speed, bool invincible,
		sf::Vector2f pos, Weapon* weapon) :
		myScene(scene), health(hp), speed(speed), invincible(invincible), myWeapon(weapon),
		Sprite(TextureManager::instance()->getRef(tex))
	{
		setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);
		this->setPosition(pos);
	};

	// for bullet
	Entity(SceneGame* scene, std::string tex, int hp, float speed, bool invincible,
		sf::Vector2f pos) :
		myScene(scene), health(hp), speed(speed), invincible(invincible),
		Sprite(TextureManager::instance()->getRef(tex))
	{
		setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);
		this->setPosition(pos);
	};

	// for pickup
	Entity(SceneGame* scene, std::string tex, sf::Vector2f pos, Weapon* weapon):
	health(1), speed(0), invincible(true),
	myScene(scene), myWeapon(weapon),
	Sprite(TextureManager::instance()->getRef(tex)){
		setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);
		this->setPosition(pos);
	};

	// destructor
	~Entity();

	//*/

	// Member functions

	virtual void update(float dt);

	virtual int getTicks() { return ticks; };

	// we leave its base function empty so the derived versions
	// can be called with unique code
	virtual void collideWith(Entity&) = 0;
	virtual void updateMovement(Movement&) = 0;
	virtual void updateWeapon(Weapon&) = 0;

   
	void setHealth(int hp) { this -> health = hp; };
	int getHealth() { return health; };
	void setSpeed(float speed) { this->speed = speed; };
	float getSpeed() { return speed; };
	void setInvincibility(bool invincible) { this->invincible = invincible; };
	bool getInvincibility() { return invincible; };

	void setScene(SceneGame *scene){myScene = scene;};
    SceneGame* getScene(){return myScene;};

	void setMovement(Movement* movement) { myMovement = movement; };
	Movement* getMovement() { return myMovement; };

	void setWeapon(Weapon* weapon) {
		if (myWeapon != nullptr){
			delete myWeapon;
			myWeapon = nullptr;
		}
		myWeapon = weapon;
	};
	Weapon* getWeapon(){ return myWeapon; };



};