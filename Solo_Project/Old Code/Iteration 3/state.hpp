#pragma once

#include "game.hpp"

class Game; // forward declare

class State{

public:
	Game* game;

	virtual void draw(const float dt) = 0;
	virtual void update(const float dt) = 0;
	virtual void handleInput() = 0;

};