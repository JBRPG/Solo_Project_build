#pragma once

#include <stack>
#include <vector>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "textureManager.hpp"

class Scene; // forward declare

class Game{

private:
	
public:

	std::stack<Scene*> scenes;

	sf::RenderWindow window;

	void pushScene(Scene* scene);
	void popScene();
	void changeScene(Scene* scene);
	Scene* peekScene();

	void gameLoop();

	void loadTextures();

	std::vector<std::string> split(const std::string &line, std::string delim,
		std::vector<std::string> &elems);

	std::vector<std::string> split(const std::string &line, std::string delim);

	Game();
	~Game();
};