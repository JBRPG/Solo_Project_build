#include "entity.hpp"


Entity::Entity(sf::Vector2f position,
	           sf::Vector2f velocity,
			   int hp, EntityType entityType,
			   const sf::Texture& texRef
			   ) : pos(position), // initialization list
			       vel(velocity),
			       hp(hp),
			       entityType(entityType),
				   eStatus{StatusID::ONSCREEN}
{
	this->eSprite.setTexture(texRef);
	this->eSprite.setOrigin(this->eSprite.getLocalBounds().width / 2,
		                    this->eSprite.getLocalBounds().height / 2);
	this->eSprite.setPosition(position);
}

Entity::Entity(sf::Vector2f position,
	int hp,
	const sf::Texture& texRef
	) : pos(position), // initialization list
	vel(sf::Vector2f(0,0)),
	hp(hp),
	entityType(EntityType::TERRAIN),
	eStatus{ StatusID::ONSCREEN }
{
	this->eSprite.setTexture(texRef);
	this->eSprite.setOrigin(this->eSprite.getLocalBounds().width / 2,
		this->eSprite.getLocalBounds().height / 2);
	this->eSprite.setPosition(position);
}

const sf::FloatRect Entity::getAABB() const{
	
	return sf::FloatRect{ pos.x + eAABB.left, pos.y + eAABB.top, eAABB.width, eAABB.height };
}

sf::Vector2f Entity::update(const float dt){

	// The entity will simply return its position
	// until more stuff gets added
	// such as entity script, commands, etc.

	return getPosition();

}

void Entity::draw(sf::RenderTarget&, sf::RenderStates) const{

}

