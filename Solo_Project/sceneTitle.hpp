#pragma once

#include <SFML\Graphics.hpp>

#include "scene.hpp"

// forward declaration
class Game;


class SceneTitle : public Scene{
private:

	sf::Font font  = sf::Font();
	sf::Text controls;

	sf::Sprite title;

	sf::View titleView;

public:

	SceneTitle(Game* game);

	virtual void draw(float dt);
	virtual void update(float dt);
	virtual void handleInput();


};