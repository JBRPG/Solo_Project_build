#pragma once

#include <SFML\Graphics.hpp>

#include "entity.hpp"
#include "sceneGame.hpp"

/*
  We will need an explosion class
  to show enemies and players being blown up when colliding with objects

  Explosions will spawn temporarily until time runs out

  Explosions are the unique entities that causes no effect
  against other entities when they collide


  for later iterations, we will despawn an explopsion entity once animation is complete


*/

class Explosion : public Entity{

private:

	float spawnTime;
	sf::Vector2i tex_frames; // organize by x and y through division
	int frames;
	int frame_wait;
	int frame_wait_set;
	int curr_frame = 0;

public:

	Explosion(const sf::Texture& tex, float spawnTime) :
		Entity(tex, 0, 0, true)
	{};


	Explosion(const sf::Texture& tex, const sf::IntRect& rect, float spawnTime) :
		Entity(tex, rect, 0, 0, true)
	{};


	Explosion(const sf::Texture&, const sf::IntRect&, int);

	void update(float dt);
	int getTicks() { return ticks; };


	// we will keep the old options of time
	void setSpawnTime(float time){ spawnTime = time; };
	float getSpawnTime() { return spawnTime; };

	// we will animate the explosion by changing frames
	void changeFrame();


    // After short time from being on screen, remove the entity
	void checkExplosion();

    // We will leave the inherited functions empty for simplicity
	void collideWith(Entity&) {};
	void updateMovement(Movement&) {};
	void updateWeapon(Weapon&) {};
	

};