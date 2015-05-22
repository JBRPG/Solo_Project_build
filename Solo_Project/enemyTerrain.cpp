#include "enemyTerrain.hpp"

#include "movement.hpp"
#include "weapon.hpp"
#include "terrain.hpp"

EnemyTerrain::EnemyTerrain(SceneGame* scene, std::string tex, int hp, float speed, bool invincible,
	sf::Vector2f pos, Weapon* weapon, Movement* movement, float grav, bool edge_turn):
	gravity(grav), turn_edge(edge_turn),
	Enemy(scene, tex, hp, speed, invincible, pos, weapon, movement)
{
	fall_dir = grav > 0 ? 1 : 0;
}

// inherited functions

void EnemyTerrain::update(float dt){

}

int EnemyTerrain::getTicks(){
	return ticks;
}

void EnemyTerrain::updateWeapon(Weapon& weapon){
	Enemy::updateWeapon(weapon);
}

// get/set functions

void EnemyTerrain::setGravity(float grav){
	gravity = grav;
	fall_dir = grav > 0 ? 1 : 0;
}

float EnemyTerrain::getGravity(){
	return gravity;
}

void EnemyTerrain::setFallSpeed(float falling){
	fallSpeed = falling;
}

float EnemyTerrain::getFallSpeed(){
	return fallSpeed;
}

void EnemyTerrain::setTurnEdge(bool tf){
	turn_edge = tf;
}

bool EnemyTerrain::getTurnEdge(){
	return turn_edge;
}

void EnemyTerrain::setOnTerrain(bool tf){
	on_terrain = tf;
}

bool EnemyTerrain::getOnTerrain(){
	return on_terrain;
}

void EnemyTerrain::collideWith(Entity& other){
	Enemy::collideWith(other);
	if (Terrain* land = dynamic_cast<Terrain*> (&other)){
		/*
		   Now there are several things to consider
		   when this type of enemy collides with terrain,
		   especially with the gravity direction:

		   if enemy lands "on top of" terrain, set falling speed to 0

		   if enemy is "under" the terrain, set position to be on "top" of terrain

		*/
	}

}
