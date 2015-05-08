#pragma once // to prevent the " : class redefinition error

#include <stack>
#include <vector>
#include <SFML/Graphics.hpp>

#include "textureManager.hpp"
#include "player.hpp"

//class Player; // forward declare

class State; // forward declaration

class Game{
private:
	void loadTextures();
	std::vector<std::string> split(const std::string &line, std::string delim,
		                            std::vector<std::string> &elems);

	std::vector<std::string> split(const std::string &line, std::string delim);

public:

	std::stack<State*> states;

	sf::RenderWindow window;
	TextureManager texmgr;

	// Collection of sprites
	sf::Sprite background;
	//sf::Sprite playerSprite;

	//Player* player;

	void pushState(State* state);
	void popState();
	void changeState(State* state);
	State* peekState();

	void gameLoop();

	Game();
	~Game();
};