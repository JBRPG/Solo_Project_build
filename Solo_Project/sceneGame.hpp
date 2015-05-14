#pragma once

#include <SFML\Graphics.hpp>

#include "scene.hpp"
#include "game.hpp"
#include "entity.hpp"
#include "movement.hpp"
#include "weapon.hpp"
#include "player.hpp"
#include "collisionGrid.hpp"


// forward declare

class BulletTemplate;

class EnemyTemplate;

class Spawner;

class Pickup;

class SceneGame : public Scene{
private:

	sf::View  background1View;
	sf::View  background2View;
	sf::View  gameView;
	sf::View  playerView;
	sf::View  hudView;


	sf::Sprite background;
	Player* player;
	bool player_dead = false;
	
	std::vector<Enemy*> enemies; // temporary variable
	std::vector <std::vector < BulletTemplate* > > bullet_Patterns;

	Pickup* powup; // we will have one pickup for testing purposes

	// Other important variables

	sf::Font font = sf::Font();
	sf::Text fpsDisplay = sf::Text("FPS:", font, 30);
	std::string framerate;

	collisionGrid gridBox;
	std::vector<Entity*> addList; // We simply add the created entities to EntityList
	std::vector<Entity*> removeList; // we match the stored entities for removal

	std::vector <Spawner*> spawner_list;

	std::vector<bool> movement_select_list;
	
	int spawn_time = 180; // For now we have it affecting all spawners

	int scene_ticks;

	sf::Vector2f scrollSpeed = sf::Vector2f(1.0f, 0);


	// functions

	void gameOver(); // if player dies, go back to start screen;

	bool withinWindow(Entity&);

	sf::IntRect getWindowBounds(Entity&);

	Spawner* makeSpawner();  // for test purposes, we make a default spawner
	Spawner* makeSpawner(Weapon*, Movement*, EnemyTemplate*, std::vector<int>);

	// For simple test purpose, we shall create a pre-fixed spawner based on time period
	void spawnTimer();


	// Procedural Content Generation-related functions

	Movement* makeMovement(sf::Vector2f);


public:
	SceneGame(Game* game);

	// Scene functions
	virtual void draw(float dt);
	virtual void update(float dt);
	virtual void handleInput();

	void populateGrid();
	void checkCollisions();

	void storeAddedEntity(Entity* entity){
		addList.push_back(entity);
	}
	void storeRemovedEntity(Entity* entity){
		removeList.push_back(entity);
	}

	// From scene
	void addEntity(Entity* entity);

	// other functions
	sf::Vector2f getScrollSpeed() { return scrollSpeed; };

	int getSpawnTime(){ return spawn_time; };
	void setSpawnTime(int time){ spawn_time = time; };

	void playerKilled();

};