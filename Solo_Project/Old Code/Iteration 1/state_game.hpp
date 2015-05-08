#pragma once

#include "state.hpp"
#include "game.hpp"
#include "player.hpp"

class Player; // forward declaration

class StateGame : public State{

public:
	
	StateGame(Game* game);

	Player* gamePlayer;

	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();


private:
	sf::View gameView;
	sf::View hudView;

};