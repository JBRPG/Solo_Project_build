#pragma once

#include <SFML\Graphics.hpp>
#include <list>
#include <memory>

#include "globalVariables.hpp"
#include "entity.hpp"

/*
  gridContainer is the class for handling the grid that's used for
  collision detection.
  The screen in divided into a grid of nGridX * nGridY cells.
  Each cell is allowed to contain a maximum number of entities.
  Cells are represneted as struct Cell with a fixed size array
  to avoid dynamic memory allocation.

  Then the grid itself is accessed via [nGridX][nGridY], which identifies
  a particular Cell. For example, the first entity in the first grid cell
  is accessed at grid.cell[0][0].items[0].

  "Clearing" a cell, which must happen in every update, is as simple
  as setting nEntities to 0. The items[] array wil still contain pointers,
  that point to entities that will be used in that cell,
  and we will not have to refer to those pointers again.
  Having a fixed array with each pointer items[] set to 0
  is much quicker than setting each pointer to nullptr or using a vector
*/

class gridContainer {

public:
	gridContainer();
	~gridContainer();

	static const int nGridX = 5;
	static const int nGridY = 5;
	static const int nMaxEntities = 100;

	struct Cell {
		Entity* items[nMaxEntities];
		int nEntities;
	};

	Cell cell[nGridX][nGridY];

	void append(int, int, Entity*);
	void reset();

//private:

};

class sceneContainer {

public:
	sceneContainer(sf::RenderWindow&);

	// returns number of non-player entities currently on scene
	unsigned int update(const float dt);
	void addEntity(std::unique_ptr<Entity> p);
	StatusID onScreen(const sf::Vector2f& pos);
	void render(sf::RenderTarget& t, sf::RenderStates s);

	// variables
	std::unique_ptr<gridContainer>  sGrid;
	sf::Vector2u sWindowSize;
	sf::Vector2u sGridSize;



private:

	// functions

	int collisionCheck();

	
	// variables
	const float                           sBufferSize = 200;
	sf::FloatRect                         sScreen;
	sf::FloatRect                         sBuffer;
	std::list<std::unique_ptr<Entity>>    sEntityList;

	Entity*                               sEnemies[gMaxEntityCount];
	Entity*                               sTerrain[gMaxEntityCount];
	int                                   sEnemyCount;
	int                                   sTerrainCount;

};