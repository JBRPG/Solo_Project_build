#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

#include "scene.hpp"
#include "game.hpp"
#include "movement.hpp"
#include "weapon.hpp"
#include "collisionGrid.hpp"


// forward declare

class Player;

class Entity;

class Enemy;

class Terrain;

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

	std::vector <sf::Sprite*> stars;

	std::vector<sf::Sound*> sound_list;
	sf::Music* gameMusic;

	Player* player;
	bool player_dead = false;
	int gameOver_wait = 180;
	
	std::vector<Enemy*> enemies; // temporary variable
	std::vector <std::vector < BulletTemplate* > > bullet_Patterns;

	Pickup* powup; // we will have one pickup for testing purposes

	// Other important variables

	sf::Font font = sf::Font();
	sf::Text fpsDisplay = sf::Text("FPS:", font, 16);
	std::string framerate;
	sf::Text score = sf::Text("Score: ", font, 16);
	std::string curr_score;
	sf::Text multiplier = sf::Text("Multi: ", font, 16);
	std::string curr_multi;

	collisionGrid gridBox;
	std::vector<Entity*> addList; // We simply add the created entities to EntityList
	std::vector<Entity*> removeList; // we match the stored entities for removal

	std::vector <Spawner*> spawner_list;
	std::vector<Terrain*> terrain_list;

	// PCG selection 
	std::vector<bool> movement_select_list;
	std::vector<int> weapon_select_list;
	std::vector<int> waypoint_select_list;
	int weapon_repeat = 1;
	int waypoint_repeat = 1;
	
	int spawn_default = 180;
	int spawn_time = spawn_default; // For now we have it affecting all spawners
	

	// difficulty-related
	int difficulty = 1;
	int difficulty_period_default = 900;
	int difficulty_period = difficulty_period_default;
	int spawn_difficulty = difficulty; // used to fluctuate difficulty of enemies when spawned
	bool spawn_diff_Fn_Tp = false;

	int scene_ticks;

	int boss_period_default = 1200;
	int boss_period = boss_period_default;
	// boss ticks is seperate so it can stop once the boss is summoned
	int boss_ticks = 0;
	bool boss_summoned = false;

	sf::Vector2f scrollSpeed = sf::Vector2f(1.0f, 0);


	// functions

	void gameOver(); // if countdown over, go back to start screen;
	void gameOverCountdown(); // if player dies, have a wait period before going to title screen

	void checkSounds();
	void deleteSounds();

	void setupStars();
	void checkStars();

	bool withinWindow(Entity&);

	sf::IntRect getWindowBounds(Entity&);

	Spawner* makeSpawner();  // for test purposes, we make a default spawner
	Spawner* makeSpawner(Weapon*, Movement*, EnemyTemplate*, std::vector<int>);

	// For simple test purpose, we shall create a pre-fixed spawner based on time period
	void spawnTimer();


	// Procedural Content Generation-related functions

	void checkDifficulty();
	void switchSpawnDifficulty();

	Movement* makeMovement(sf::Vector2f); // make movements
	EnemyTemplate* makeEnemy(Movement*, Weapon*); // make generic type of enemies
	Weapon* makeWeapon(); // make weapons
	void makePickup(); // make pickups
	void makeTerrain(); // make terrain
	void makeWaypoints(); // make waypoints
	void makeTerrainEnemy(Terrain*); // make terrain-type enemies
	void makeBoss(); // make a boss
	Weapon* setup_bullets(std::string&, std::vector<BulletTemplate*>&);


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

	int getDifficulty();
	void setDifficullty(int);

	void playerKilled();

	void playSound(std::string);

	void bossDefeated();
	bool summonBoss();

	Player* getPlayer();

};