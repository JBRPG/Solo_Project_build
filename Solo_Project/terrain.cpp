#include "terrain.hpp"

#include "textureManager.hpp"
#include "sceneGame.hpp"

#include "player.hpp"
#include "enemyTerrain.hpp"
#include "bullet.hpp"


Terrain::Terrain() :
Entity(TextureManager::instance()->getRef("metal wall"), 100, 0, true)
{}

Terrain::Terrain(const sf::Texture& tex, int hp, float speed, bool invincible) :
Entity(tex, hp, speed, invincible){}

Terrain::Terrain(const sf::Texture& tex, sf::IntRect& rect,
	int hp, float speed, bool invincible) :
	Entity(tex, rect, hp, speed, invincible){}


// for collision with terrain

// Destroy the player if touching the terrain

// destroy the bullets 

/*
The enemies are invincible to the terrain

The ground type enemies must be placed "above"
the terrain depending on gravity direction
(falling up, must not fall through the top
falling down, must not fall through the bottom)

*/


void Terrain::collideWith(Entity& other){
	if (Player* player = dynamic_cast<Player*> (&other)){

	}
	else if (Bullet* bullet = dynamic_cast<Bullet*> (&other)){

	}
	else if (EnemyTerrain* landEnemy = dynamic_cast<EnemyTerrain*> (&other)){

	}

}

