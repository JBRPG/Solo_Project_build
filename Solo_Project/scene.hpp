#pragma once

#include <SFML\Graphics.hpp>

#include <memory>
#include <vector>

#include "game.hpp"
#include "entity.hpp"

class Game; // forward declare

class Scene{
private:

	
protected:

	std::vector<Entity*> EntityList;


public:

	Game* game;


	virtual void draw(float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void handleInput() = 0;

	virtual void addEntity(Entity*);
	virtual void removeEntity(Entity*);
	virtual void cleanEntityList();
	virtual int  getEntitysize();
	virtual Entity* getEntity(int);


};