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

	int multiplier;
	int score;
	int highscore;
	bool game_over = true;
	bool atStart = true;

	
public:

	std::stack<Scene*> scenes;

	sf::RenderWindow window;

	void pushScene(Scene* scene);
	void popScene();
	void changeScene(Scene* scene);
	Scene* peekScene();

	void gameLoop();

	void loadTextures();
	void loadSounds();

	std::vector<std::string> split(const std::string &line, std::string delim,
		std::vector<std::string> &elems);

	std::vector<std::string> split(const std::string &line, std::string delim);

	void increaseMultiplier();
	void setMultiplier(int);
	int getMultiplier();

	void increaseScore(int);
	void setScore(int);
	int getScore();

	void setHiScore();
	int getHiScore();

	bool getAtStartScreen();
	void setAtStartScreen(bool);

	bool getGameOver();
	void setGameOver(bool);

	Game();
	~Game();
};