#pragma once

#include "entity.hpp"

class AnimatedParticle : public Entity{
private:

	float spawnTime;
	sf::Vector2i tex_frames; // organize by x and y through division
	int frames;
	int frame_wait;
	int frame_wait_set;
	int curr_frame = 0;


public:

	AnimatedParticle(const sf::Texture&, const sf::IntRect&, int);

	void update(float dt);
	int getTicks() { return ticks; };


	void changeFrame();
	void resetAnimation();

	// We will leave the inherited functions empty for simplicity
	void collideWith(Entity&) {};
	void updateMovement(Movement&) {};
	void updateWeapon(Weapon&) {};


};