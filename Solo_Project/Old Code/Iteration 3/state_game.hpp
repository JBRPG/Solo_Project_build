#pragma once

#include <SFML\Graphics.hpp>
#include <vector>

#include "state.hpp"
#include "game.hpp"
#include "player.hpp"
#include "entity.hpp"
#include "globalVariables.hpp"
#include "level.hpp"


class StateGame : public State{

public:
	
	StateGame(Game& game);

	//Player* gamePlayer;
	std::vector<Entity> gameEntities;

	virtual void draw(const float dt);
	virtual void update(const float dt);
	virtual void handleInput();


private:
	
	Level mLevel;

};