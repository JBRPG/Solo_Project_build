#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "game.hpp"
#include "state.hpp"
#include "textureManager.hpp"

void Game::loadTextures(){

	std::string current_line;

	std::ifstream target_file("scripts/texture_list.txt");

	/*
	  texture_list will have two tokens per line,
	  string name (0), and filename (1), seprated by ", "
	*/

	if (target_file.is_open()){

		while (std::getline(target_file, current_line)){
			std::vector<std::string> elems = Game::split(current_line, ", ");
			texmgr.loadTexture(elems.at(0), elems.at(1));
		}
		target_file.close();
	}

	/*
	texmgr.loadTexture("playerSprite", "media/images/player.png");
	texmgr.loadTexture("background", "media/images/background.png");
	texmgr.loadTexture("metal wall", "media/images/Metal.png");
	//*/

	else{
		std::cout << "Cannot open file. Close program";
		this->window.close();
	}
}

void Game::pushState(State* state)
{
	this->states.push(state);

	return;
}

void Game::popState()
{
	delete this->states.top();
	this->states.pop();

	return;
}

void Game::changeState(State* state)
{
	if (!this->states.empty())
		popState();
	pushState(state);

	return;
}

State* Game::peekState()
{
	if (this->states.empty()) return nullptr;
	return this->states.top();
}

void Game::gameLoop()
{
	sf::Clock clock;

	while (this->window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();

		if (peekState() == nullptr) continue;
		peekState()->handleInput();
		peekState()->update(dt);
		this->window.clear(sf::Color::Black);
		peekState()->draw(dt);
		this->window.display();
	}
}

Game::Game()
{
	this->loadTextures();

	this->window.create(sf::VideoMode(800, 600), "Solo Project Ver 0.0");
	this->window.setFramerateLimit(60);

	this->background.setTexture(this->texmgr.getRef("background"));
}

Game::~Game()
{
	while (!this->states.empty()) popState();
}

// Split strings for file extraction purposes


std::vector<std::string> Game::split(const std::string &line, std::string delim,
	std::vector<std::string> &elems){

	unsigned int start = 0U;
	unsigned int end = line.find(delim);

	while (end != std::string::npos){
		elems.push_back(line.substr(start, end - start));
		start = end + delim.length();
		end = line.find(delim, start);
	}
	elems.push_back(line.substr(start, end));

	return elems;
}

std::vector<std::string> Game::split(const std::string &line, std::string delim){
	std::vector<std::string> elems;
	Game::split(line, delim, elems);
	return elems;
}