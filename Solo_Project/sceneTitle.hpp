#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

#include "scene.hpp"

// forward declaration
class Game;


class SceneTitle : public Scene{
private:

	sf::Sound player_destroyed;

	sf::Font font  = sf::Font();
	sf::Text controls;
	sf::Text hiscore;

	sf::Sprite title;

	sf::View titleView;

public:

	SceneTitle(Game* game);

	void play_playerDefeat();

	virtual void draw(float dt);
	virtual void update(float dt);
	virtual void handleInput();


};