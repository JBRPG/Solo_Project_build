#pragma once

#include <SFML\Graphics.hpp>
#include "textureManager.hpp"
#include "globalVariables.hpp"

/* Entity is a "flexible" class that would store common data that would
   allow specific classes to expand its use(i.e Player, Enemy, etc.).
*/

// Player properties will go there later
// since it has common properties that are shared with
// others like terrain, enemies, and bullets shot by player and enemy

class Entity : public sf::Drawable, public sf::Transformable{

public:

	// Constructors
	Entity(
		sf::Vector2f position,
		sf::Vector2f velocity,
		int hp,             // for enemy units, player, and some terrain
		EntityType entityType,     // type of entity (enemy, player, terrain, etc.)
		const sf::Texture& texRef // texture reference
		);

	Entity(
		sf::Vector2f position,
		int hp,             // for enemy units, player, and some terrain
		const sf::Texture& texRef // texture reference
		);
	//~Entity();

	// functions
	virtual sf::Sprite getSprite() { return eSprite; };

	virtual const sf::FloatRect getAABB() const; // Returns AABB

	void setVelocity(sf::Vector2f vel) { this->vel = vel; };
	sf::Vector2f getVelocity() { return this->vel; };

	void setPosition(sf::Vector2f pos) { this->pos = pos; };
	sf::Vector2f getPosition() { return this->pos; };

	void setHP(int hp) { this->hp = hp; };
	int getHP() { return this->hp; };

	void setEntityType(EntityType entityType) { this->entityType = entityType; };
	EntityType getEntityType() { return this->entityType; }

	void setStatus(StatusID stat) { this->eStatus = stat; };
	StatusID getStatus() { return this->eStatus; };

	sf::Vector2f update(const float dt); // returns position




private:

	// functions
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;


	// parameters
	sf::Vector2f pos;
	sf::Vector2f vel;
	int hp;
	EntityType entityType;

	// other variables
	sf::Sprite      eSprite;
	sf::FloatRect   eAABB;
	float           eRotSpeed; // rotates that many degrees per second
	StatusID        eStatus; // status of entity

};