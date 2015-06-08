#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

#include "scene.hpp"
#include "particle_animated.hpp"

// forward declaration
class Game;


class SceneTitle : public Scene{
private:


	sf::Font font  = sf::Font();
	sf::Text controls;
	sf::Text hiscore;

	AnimatedParticle* game_title;

	sf::View titleView;

public:

	SceneTitle(Game* game);

	void play_playerDefeat();

	virtual void draw(float dt);
	virtual void update(float dt);
	virtual void handleInput();


};