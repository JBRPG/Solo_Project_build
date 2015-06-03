#include "sceneGame.hpp"

#include <sstream>
#include <time.h>
#include <cstdio>
#include <cstdlib>

#include "input.hpp"
#include "soundPlayer.hpp"

#include "spawner.hpp"

#include "pickup.hpp"

#include "player.hpp"
#include "entity.hpp"
#include "enemy.hpp"
#include "terrain.hpp"
#include "enemyTerrain.hpp"
#include "boss.hpp"


SceneGame::SceneGame(Game* game){

	this->game = game; // need this to run the game.


	// setup the collision box
	// gridBox must be larger scale of the game window
	gridBox.setSize(game->window.getSize().x * 1.5,
		            game->window.getSize().y * 1.5);
	// make sure game window is at center of collision box
	
	gridBox.setOrigin(-(int(game->window.getSize().x / 4)),
		              -(int(game->window.getSize().y / 4)));

	font.loadFromFile("media/PressStart2P-Regular.ttf");
	fpsDisplay.setPosition(50, 0);
	score.setPosition(500, 0);

	// Set up the view layers

	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	this->background1View.setSize(pos);
	this->background2View.setSize(pos);
	this->gameView.setSize(pos);
	this->playerView.setSize(pos);
	this->hudView.setSize(pos);
	pos *= 0.5f;
	this->background1View.setCenter(pos);
	this->background2View.setCenter(pos);
	this->gameView.setCenter(pos);
	this->playerView.setCenter(pos);
	this->hudView.setCenter(pos);

	// Initialize the background

	background = sf::Sprite(TextureManager::instance()->getRef("background"));

	// setup the stars
	setupStars();

	// Player will always be initalized at the start of the game

	player = new Player(TextureManager::instance()->getRef("playerAnimate"),
		sf::IntRect(0,0,60,32),
		1, 8, false, 3);
	player->setPosition(sf::Vector2f(100, 200));
	player->setWeapon(new Weapon());

	addEntity(player);

	// set up randomization and PCG elements

	// movement list contains 4 types of movement as of now
	movement_select_list = std::vector<bool>(4, false);
	waypoint_select_list = std::vector<int>(6, 0);
	weapon_select_list = std::vector<int>(4, 0);

}

void SceneGame::draw(float dt){

	this->game->window.setView(background1View);
	this->game->window.draw(background);
	

	this->game->window.setView(gameView);

	for (auto star : stars){
		this->game->window.draw(*star);
	}

	for (auto land : terrain_list){
		game->window.draw(*land);
	}

	// Test: make sure enemies are not covered by terrain
	for (int i = 0; i < getEntitysize(); ++i){
		auto idxSprite = getEntity(i);
		Terrain* land = dynamic_cast<Terrain*> (idxSprite);
		if (land == nullptr){
			this->game->window.draw(*idxSprite);
		}
	}

	sf::Vector2f center = sf::Vector2f(game->window.getSize());
	center *= 0.5f;
	this->game->window.draw(fpsDisplay);
	this->game->window.draw(score);

}

void SceneGame::update(float dt){
	++scene_ticks;
	///*
	std::stringstream strDisplay;
	strDisplay << "FPS: " << int(1 / dt);
	framerate = strDisplay.str();
	fpsDisplay.setString(framerate);
	fpsDisplay.move(scrollSpeed);

	std::stringstream scoreDisplay;
	scoreDisplay << "Score: " << game->getScore();
	curr_score = scoreDisplay.str();
	score.setString(curr_score);
	score.move(scrollSpeed);


	checkStars();
	checkDifficulty();


	if (!boss_summoned){
		makeTerrain();
		spawnTimer();
		boss_ticks++;
		summonBoss();
	}
	makePickup();

	for (auto spawn : spawner_list){
		spawn->setSpawnLocation(game->window.mapPixelToCoords(spawn->getWindowCoords()));
		spawn->update();
	}
	//*/

	///update the entities inside the current EntityList
	for (int i = 0; i < getEntitysize(); ++i){
		Entity* idxEntity = getEntity(i);
		idxEntity->update(dt);
		
	}

	// Add the created entities

	for (auto entity : addList){
		addEntity(entity);
	}

	// Now check collisions

	checkCollisions();

	
	// if there are no entities to erase, update is complete
	if (!removeList.empty()){

		// remove the destroyed entities
		// must iterate backwards for both vector-based entity lists

		for (int j = removeList.size() - 1; j >= 0; --j){
			// check the entities one by one
			for (int k = EntityList.size() - 1; k >= 0; --k){

				// remove from entity list if searched entity is found

				// Since iterators act as pointers, they can be dereferenced to
				// access the current element being searched
				// to make sure they point to the same Entity

				Entity* p = removeList[j]; // pointer to entities in removeList
				Entity* q = EntityList[k]; // pointer to entities in EntityList

				if (p == q){
					// remove entity from entityList

					// check if the entity is terrain, remove it from terrain list if true
					Terrain* land = dynamic_cast<Terrain*>(p);
					if (p == land){
						int pos = std::find(terrain_list.begin(), terrain_list.end(), land) - terrain_list.begin();
						terrain_list.erase(terrain_list.begin() + pos);
					}

					delete p;
					p = nullptr;
					EntityList.erase(EntityList.begin() + k);
					

					break;
				}

			}

		}
	}

	for (int i = spawner_list.size() - 1; i >= 0; --i){
		
		//*
		Spawner* spawn = spawner_list[i];
		if (spawn->getSpawnLimit() == 0){
			delete spawn;
			spawner_list.erase(spawner_list.begin() + i);
		}

		//*/
	}

	//empty the containers of added and removed entities 
	addList.clear();
	removeList.clear();

	checkSounds();

	gameView.move(scrollSpeed);

	// add in scrolling stars later

	gameOverCountdown();


}

void SceneGame::handleInput(){
	sf::Event event;

	while (this->game->window.pollEvent(event)){
		switch (event.type){
		  case sf::Event::Closed:
		  {
			  this->game->window.close();
			  break;
		  }

		  case sf::Event::KeyPressed:
		  {
			  if (event.key.code == sf::Keyboard::Escape){
				  this->game->window.close();
			  }
			  break;
		  }

		  default:
		  {
			  break;
		  }

		}
	}
}


/*
  These collision-based functions are responsible
  for managing all the entities on screen.
  
  populateGrid will place the entities at the appropriate cells.

  checkCollisions will compare entities within occupied cells.
  If the two unique entities actually collide, then do the collision
  work between the two.


*/

void SceneGame::populateGrid(){

	/*
	  I must work on this function since I got to convert the coordinate points from the
	  the current viewpoint to the render window (aka the render target) coordinate in pixels

	*/

	gridBox.reset();

	// Set up the rectangle to represent the cell.
	// Default is at top left corner
	sf::IntRect cellRect = sf::IntRect
		(sf::Vector2i(0,0),sf::Vector2i(
				gridBox.getSize().x/gridBox.slicesX,
				gridBox.getSize().y/gridBox.slicesY));

    // Now we check each entity if they are inside the grid

	for (int i = 0; i < getEntitysize(); ++i){

		// if entity is outside the game window,
		// store the entity and continue with the rest of the entities

		if (!withinWindow(*getEntity(i))){
			storeRemovedEntity(getEntity(i));
			continue;
	    }



		for (auto x = 0; x < gridBox.slicesX; ++x){
			for (auto y = 0; y < gridBox.slicesY; ++y){

				// Set up the 4 corners for the cell's area

				// had to do float casting for more precise cells
				// change into the gridBox's coordinates

				cellRect.left = (gridBox.getSize().x) *
					            (float(x) / float(gridBox.slicesX))
								+ gridBox.getOrigin().x;
				cellRect.top = (gridBox.getSize().y) *
					            (float(y) / float(gridBox.slicesY))
								+ gridBox.getOrigin().y;
				cellRect.width = (gridBox.getSize().x) / (gridBox.slicesX);
				cellRect.height = (gridBox.getSize().y) / (gridBox.slicesY);


				// Add the entitiy to the cell if they touch

				sf::IntRect entity_window = getWindowBounds(*getEntity(i));
				if (entity_window.intersects(cellRect)){
					gridBox.append(x, y, getEntity(i));
				}


			}
		}


	}
	
}

void SceneGame::checkCollisions(){

	populateGrid();


	for (auto i = 0; i < gridBox.slicesX; ++i){
		for (auto j = 0; j < gridBox.slicesY; ++j){
			if (gridBox.cell[i][j].nEntities < 2) continue;

			for (auto k = 0; k < gridBox.cell[i][j].nEntities; ++k){
				for (auto l = 0; l < gridBox.cell[i][j].nEntities; ++l){

					// Set up the pointers and compare them
					auto p = gridBox.cell[i][j].items[k];
					auto q = gridBox.cell[i][j].items[l];
					if (p == q) continue; // we do not want the same pointer

					if (p->getGlobalBounds().intersects(q->getGlobalBounds())){

						// Do a series of collisions depending on the specific entities
						p->collideWith(*q);

					}


				}
			}
		}
	}

}

/*
   Functions derived from Scene
*/

void SceneGame::addEntity(Entity* entity){
	entity->setScene(this);
	EntityList.push_back(entity);
}

bool SceneGame::withinWindow(Entity& entity){
	sf::Vector2i tl_pixels = this->game->window.mapCoordsToPixel(
		sf::Vector2f(entity.getGlobalBounds().left, entity.getGlobalBounds().top), gameView);
	sf::Vector2i tr_pixels = this->game->window.mapCoordsToPixel(
		sf::Vector2f(entity.getGlobalBounds().left + entity.getGlobalBounds().width,
		entity.getGlobalBounds().top), gameView);

	sf::IntRect entity_pixels = sf::IntRect(tl_pixels,
		sf::Vector2i(entity.getTextureRect().width,
		entity.getTextureRect().height));

	// change this to the collision box so entities won't have to be deleted when outside window

	sf::IntRect windowRect = sf::IntRect(gridBox.getOrigin().x, gridBox.getOrigin().y,
		                                 gridBox.getSize().x, gridBox.getSize().y);

	return entity_pixels.intersects(windowRect);



}

sf::IntRect SceneGame::getWindowBounds(Entity& entity){
	sf::Vector2i entity_pixelTL = game->window.mapCoordsToPixel(
		sf::Vector2f(entity.getGlobalBounds().left,
		entity.getGlobalBounds().top));

	return sf::IntRect(entity_pixelTL,
		sf::Vector2i(entity.getGlobalBounds().width,
		entity.getGlobalBounds().height));

}

int SceneGame::getDifficulty(){
	return difficulty;
}

void SceneGame::setDifficullty(int diff){
	difficulty = diff;
}

// We shall test spanwer creation with fixed variables
Spawner* SceneGame::makeSpawner(){
	Spawner* newSpawn;

	
	Weapon* enemy_weapon = makeWeapon();

	// this is where you assign the spawner through window coordinates
	// make sure that the initial location is randomized

	int randomWindowY = rand() % 200 + 200;

	sf::Vector2i spawn_window_coords = sf::Vector2i(850, randomWindowY);

	sf::Vector2f window_to_map_spawn = game->window.mapPixelToCoords(spawn_window_coords, gameView);


	Movement* enemy_movement = makeMovement(window_to_map_spawn);

	int spawn_amount = rand() % difficulty + 1;

	newSpawn = new Spawner(
		new Weapon(*enemy_weapon),
		new Movement(*enemy_movement),
		makeEnemy(enemy_movement, enemy_weapon), {30, spawn_amount}, spawn_window_coords);


	return newSpawn;
}

Spawner* SceneGame::makeSpawner(Weapon* weapon, Movement* movement, EnemyTemplate* enemy, std::vector<int> params){
	Spawner* newSpawn = new Spawner(weapon, movement, enemy, params);

	return newSpawn;
}

void SceneGame::spawnTimer(){
	if (scene_ticks % spawn_time == 0){
		Spawner* newSpawn =  makeSpawner();
		spawner_list.push_back(newSpawn);
	}
}

// create a movement based on semi-random procedure, 
// which all movements are eventually selected and process repeats
Movement* SceneGame::makeMovement(sf::Vector2f vertex){
	// a random value will be called based on Move select list's index value
	// 0 is always waypoints
	// all other are formula movements

	Movement* newMove = nullptr;

	int idx = rand() % movement_select_list.size();

	bool all_used = true;
	for (bool used : movement_select_list){
		if (!used){
			all_used = false;
			break;
		}
	}
	// reset the movement selection if they are all used once 
	if (all_used){
		std::fill(movement_select_list.begin(), movement_select_list.end(), false);
	}

	// if not all movements used, then select unclaimed spot
	while (movement_select_list[idx]){
		idx = (idx + 1) % movement_select_list.size();
	}
	movement_select_list[idx] = true;

	std::string wordKey;

	switch (idx){
	case (1) :
		wordKey = "straight";
		break;
	case (2) :
		wordKey = "circle";
		break;
	case (3) :
		wordKey = "sine";
		break;
	default:
		wordKey = "waypoint"; 
		break;
	}

	if (wordKey == "waypoint"){
		// make movement with waypoints

		// initalize Movement

		// right now for testing purposes, we have a predefined waypoint pattern.
		// Will explore more options for creating unique waypoint patterns later.

		std::vector<sf::Vector2f> waypoints = {
			sf::Vector2f(-600, 0),
			sf::Vector2f(-600, -200),
			sf::Vector2f(0, -200),
		};

		newMove = new Movement(vertex, waypoints, false);
	}
	else {
		// make movement based on mathematic formulas
		// I normally do not like using magic numbers
		// but I am using them for test purposes

		if (wordKey == "straight"){
			float angle = float(rand() % 30 + 120);
			newMove = new Movement(wordKey, vertex, { angle });

		}
		else if (wordKey == "circle"){
			float radius = float(rand() % 100 + 21);
			newMove = new Movement(wordKey, vertex, { radius });


		}
		else if (wordKey == "sine"){
			float amplitude = float(rand() % 50 + 1);
			float period = float(rand() % 10 + 1) / 300;
			newMove = new Movement(wordKey, vertex, { amplitude, period });

		}


	}

	return newMove;
}


// Will work on later

// Main focus today!

EnemyTemplate* SceneGame::makeEnemy(Movement* movement, Weapon* weapon){

	EnemyTemplate* new_Enemy = nullptr;

	// determine the enemy graphic based on weapon type
	if (weapon->getKeyword() == "single"){


		new_Enemy = new EnemyTemplate(this, "enemySprite",1,8,false,movement->getVertex());

	}

	else if (weapon->getKeyword() == "rapid_enemy"){


		new_Enemy = new EnemyTemplate(this, "enemy0Sprite", 1, 8, false, movement->getVertex());

	}

	else if (weapon->getKeyword() == "sequence_enemy"){


		new_Enemy = new EnemyTemplate(this, "enemy2Sprite", 1, 8, false, movement->getVertex());

	}
	else if (weapon->getKeyword() == "sequence_multi_enemy"){


		new_Enemy = new EnemyTemplate(this, "enemy1Sprite", 1, 8, false, movement->getVertex());

	}

	return new_Enemy;
}


// Will work on later
// this will be for the enemy


// randomly select a weapon type for an enemy
Weapon* SceneGame::makeWeapon(){

	// Will add in difficulty-based adjustment AFTER getting test run complete
	Weapon* new_weapon = nullptr;

	// determine through random drawing, but allow all possiblities at least multiple times
	
	int idx = rand() % weapon_select_list.size();

	bool all_used = true;
	for (int count : weapon_select_list){
		if (count != weapon_repeat){
			all_used = false;
			break;
		}
	}
	// reset the weapon selection if they are all used once 
	if (all_used){
		std::fill(weapon_select_list.begin(), weapon_select_list.end(), 0);
	}

	while (weapon_select_list[idx] == weapon_repeat){
		idx = (idx + 1) % weapon_select_list.size();
	}
	weapon_select_list[idx]++;


	// now select a weapon
	std::string wordKey;

	switch (idx){
	case (1) :
		wordKey = "rapid_enemy";
		break;
	case (2) :
		wordKey = "sequence_enemy";
		break;
	case (3) :
		wordKey = "sequence_multi_enemy";
		break;
	default:
		wordKey = "single";
		break;
	}


	// Right now we are only testing the most basic version
	// before going onto higher difficulty versions


	std::vector<BulletTemplate*> bullets;

	new_weapon = setup_bullets(wordKey, bullets);




	return new_weapon;
}

void SceneGame::makePickup(){

	// after a few enemy spawns, spawn a pickup with a randomly chosen weapon
	if (scene_ticks % (spawn_time * (2 + difficulty)) == 0){
		
		int weapons = 6;
		int weapon_choose = rand() % weapons;

		std::vector<BulletTemplate*> player_weapon;
		Weapon* pickup_weapon = nullptr;
		Pickup* powItem = nullptr;

		sf::Vector2f window_spawn = game->window.mapPixelToCoords(
			sf::Vector2i(game->window.getSize().x, game->window.getSize().y));

		if (weapon_choose == 0){
			// single shot
			player_weapon.push_back(new BulletTemplate("bulletPlayer",1,15,false,0));
			pickup_weapon = new Weapon(player_weapon, "single", 60);
			powItem = new Pickup(this, "pickup1", sf::Vector2f(window_spawn.x + 50, 300), pickup_weapon);
		}
		else if (weapon_choose == 1){
			// tail shot
			player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 15, false, 0));
			player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 15, false, 180));
			pickup_weapon = new Weapon(player_weapon, "single", 60);
			powItem = new Pickup(this, "pickupHorizontal", sf::Vector2f(window_spawn.x + 50, 300), pickup_weapon);
		}
		else if (weapon_choose == 2){
			// Back double
			player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 15, false, 190));
			player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 15, false, 170));
			pickup_weapon = new Weapon(player_weapon, "single", 60);
			powItem = new Pickup(this, "pickup2Back", sf::Vector2f(window_spawn.x + 50, 300), pickup_weapon);
		}
		else if (weapon_choose == 3){
			// Front double
			player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 15, false, 10));
			player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 15, false, -10));
			pickup_weapon = new Weapon(player_weapon, "single", 60);
			powItem = new Pickup(this, "pickup2Front", sf::Vector2f(window_spawn.x + 50, 300), pickup_weapon);
		}
		else if (weapon_choose == 4){
			// vertical shot
			player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 15, false, 90));
			player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 15, false, 270));
			pickup_weapon = new Weapon(player_weapon, "single", 60);
			powItem = new Pickup(this, "pickupVertical", sf::Vector2f(window_spawn.x + 50, 300), pickup_weapon);
		}
		else if (weapon_choose == 5){
			// Front Triple
			player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 20, false, 0));
			player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 20, false, 15));
			player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 20, false, -15));
			pickup_weapon = new Weapon(player_weapon, "single", 60);
			powItem = new Pickup(this, "pickup3Front", sf::Vector2f(window_spawn.x + 50, 300), pickup_weapon);
		}
		this->addEntity(powItem);

	}

}

Weapon* SceneGame::setup_bullets(std::string& word, std::vector<BulletTemplate*>& _bullets){

	Weapon* new_weapon = nullptr;

	if (word == "rapid_enemy"){
		if (difficulty == 1){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			new_weapon = new Weapon(_bullets, word, 60, { 6, 12 });
		}
		else if (difficulty == 2){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 5));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -5));
			new_weapon = new Weapon(_bullets, word, 60, { 6, 12 });
		}
		else if (difficulty == 3){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 180));
			new_weapon = new Weapon(_bullets, word, 60, { 6, 12 });
		}
		else if (difficulty == 4){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 30));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -30));
			new_weapon = new Weapon(_bullets, word, 60, { 6, 12 });
		}
		else if (difficulty == 5){

			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 90));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 180));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 270));
			new_weapon = new Weapon(_bullets, word, 60, { 6, 12 });
		}
	}
	else if (word == "sequence_enemy"){
		if (difficulty == 1){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 5));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -5));
			new_weapon = new Weapon(_bullets, word, 60, { 12 });
		}
		else if (difficulty == 2){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 10));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -10));
			new_weapon = new Weapon(_bullets, word, 60, { 12 });
		}
		else if (difficulty == 3){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 15));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -15));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			new_weapon = new Weapon(_bullets, word, 60, { 12 });
		}
		else if (difficulty == 4){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -10));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -5));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 5));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 10));
			new_weapon = new Weapon(_bullets, word, 60, { 12 });
		}
		else if (difficulty == 5){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 315));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 45));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 270));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 90));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 135));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 225));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 180));
			new_weapon = new Weapon(_bullets, word, 60, { 12 });
		}
	}
	else if (word == "sequence_multi_enemy"){
		if (difficulty == 1){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -5));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 5));
			new_weapon = new Weapon(_bullets, word, 30, { 20, 2 });
		}
		else if (difficulty == 2){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 180));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 90));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 270));
			new_weapon = new Weapon(_bullets, word, 30, { 20, 2 });
		}
		else if (difficulty == 3){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -15));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 15));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 180));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 195));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			new_weapon = new Weapon(_bullets, word, 30, { 20, 3 });
		}
		else if (difficulty == 4){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 180));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 90));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 270));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 45));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -45));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 225));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 135));
			new_weapon = new Weapon(_bullets, word, 30, { 20, 2 });
		}
		else if (difficulty == 5){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 5));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -5));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 175));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 185));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 85));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 95));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 265));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 275));
			new_weapon = new Weapon(_bullets, word, 30, { 20, 4 });
		}
	}
	else if (word == "single"){
		if (difficulty == 1){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			new_weapon = new Weapon(_bullets, word, 60);
		}
		else if (difficulty == 2){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 5));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -5));
			new_weapon = new Weapon(_bullets, word, 60);
		}
		else if (difficulty == 3){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 30));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -30));
			new_weapon = new Weapon(_bullets, word, 60);
		}
		else if (difficulty == 4){
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 90));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 180));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 270));
			new_weapon = new Weapon(_bullets, word, 60);
		}
		else if (difficulty == 5){

			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 45));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 90));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -45));
			_bullets.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -90));
			new_weapon = new Weapon(_bullets, word, 60);
		}
	}

	return new_weapon;
}

void SceneGame::makeTerrain(){
	// make terrain 1/2 of the time
	if (scene_ticks % spawn_time == 0 &&
		rand() % 2 == 0){

		int pow_width = (rand() % 4) + 5;
		int pow_height = (rand() % 4) + 5;
		int terr_width = int(pow(2, pow_width));
		int terr_height = int(pow(2, pow_height));
		bool top_bot = rand() % 2 == 1 ? true : false;

		Terrain* newBlock = new Terrain(TextureManager::instance()->getRef("terrain"),
			sf::IntRect(0,0,terr_width,terr_height), 100, 0, 1);
		terrain_list.push_back(newBlock);
		addEntity(newBlock);

		sf::Vector2f window_size = game->window.mapPixelToCoords(
			sf::Vector2i(game->window.getSize().x, game->window.getSize().y), gameView);

		newBlock->setPosition(window_size.x + newBlock->getLocalBounds().width / 2,
			(window_size.y * top_bot) - ((2 * (top_bot) - 1) * newBlock->getLocalBounds().height / 2));
		
		// **later** will add in terrain enemy with given position of the terrain
		makeTerrainEnemy(newBlock);

	}
}

void SceneGame::makeTerrainEnemy(Terrain* land){
	if (rand() % 3 == 0){
		// create land enemy with the terrain's info


		// if the terrain is placed on top of screen, fall up
		// otherwise if terrain is placed at bottom, fall down
		float gravity = -0.8;
		if (land->getGlobalBounds().top > (game->window.getSize().y / 2)){
			gravity *= -1;
		}
		int _flip = gravity > 0 ? 1 : -1;

		// for testing purposes, we will have one type of enemy that will stay on the ground
		// with pre-defined weapon and movement

		Movement* land_move = new Movement("stay", sf::Vector2f(0, 0), {0});
		std::vector<BulletTemplate*> land_shoot_bullets;
		land_shoot_bullets.push_back(new BulletTemplate("bulletEnemy",
			                 1, 10, false, (30 * _flip) ));
		Weapon* land_shoot = new Weapon(land_shoot_bullets, "single", 60);

		EnemyTerrain* landEnemy = new EnemyTerrain(this,"enemyLand",1,0,false,
			    sf::Vector2f(land->getPosition().x, land->getPosition().y -
				              (land->getGlobalBounds().height * _flip)),
				land_shoot, land_move,gravity,false);
		landEnemy->setScale(1,_flip);
		addEntity(landEnemy);

	}
}

void SceneGame::makeBoss(){

	// since we only are going to have one boss, it's best
	// to hardcode necessary items

	// setup location
	sf::Vector2f boss_spawn = game->window.mapPixelToCoords
		(sf::Vector2i(game->window.getSize().x, game->window.getSize().y / 2));

	// the boss properties

	std::vector<int> boss_phases = { 30, 10 };
	std::vector<Weapon*> boss_weapons;
	std::map<int, std::vector<BulletTemplate*>> bullets_list;

	// 3 way - rapid
	bullets_list[0].push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
	bullets_list[0].push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 10));
	bullets_list[0].push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -10));


	// 7 way - Multi-Sequence 4 shot
	bullets_list[1].push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 0));
	bullets_list[1].push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 10));
	bullets_list[1].push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 20));
	bullets_list[1].push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 30));
	bullets_list[1].push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -10));
	bullets_list[1].push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -20));
	bullets_list[1].push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -30));

	boss_weapons.push_back(new Weapon(bullets_list[0], "rapid_enemy", 60,
	{ 10, 30 }));
	boss_weapons.push_back(new Weapon(bullets_list[1], "sequence_multi_enemy", 40,
	{10,4}));
	
	Movement* boss_move = new Movement("bounds", boss_spawn, {90,1});

	Boss* newBoss = new Boss(this,"Boss0",30,6,true,boss_spawn, new Weapon(*boss_weapons[0]),
		boss_move, "Boss0", boss_weapons, {30,10},60);
	addEntity(newBoss);



}

// Will handle it later
void SceneGame::makeWaypoints(){

}

void SceneGame::checkDifficulty(){
	int difficultyPeriod = 600;
	int difficultyMax = 5;

	if (scene_ticks % difficultyPeriod == 0 && difficulty < difficultyMax){
		difficulty++;
	}


}

void SceneGame::setupStars(){
	
	// lets create a fixed number of stars
	int star_count = 50;

	for (int i = 0; i < star_count; ++i){
		stars.push_back(new sf::Sprite(TextureManager::instance()->getRef("star")));
	}

	for (int i = 0; i < stars.size(); ++i){
		sf::Sprite* star = stars[i];
		int pix_x = rand() % game->window.getSize().x + 1;
		int pix_y = rand() % game->window.getSize().y + 1;
		sf::Vector2i star_window = sf::Vector2i(pix_x, pix_y);
		sf::Vector2f star_bgview = game->window.mapPixelToCoords(star_window, gameView);
		star->setPosition(star_bgview);

	}
}

void SceneGame::checkStars(){
	for (auto star : stars){
		sf::Vector2i star_window = game->window.mapCoordsToPixel(star->getPosition());
		sf::Vector2f star_warp_coord = game->window.mapPixelToCoords(
			sf::Vector2i(game->window.getSize().x, 0)) +
			sf::Vector2f(star->getLocalBounds().width, star->getPosition().y);


		if (star_window.x < - star->getLocalBounds().width){
			star->setPosition(star_warp_coord);
		}
		else {
			star->move(sf::Vector2f(0, 0) - scrollSpeed);
		}
	}

}

// destroy all the enemies and spawners
// then remove this scene to go back to title screen

void SceneGame::gameOver(){
	if (!player_dead) return;

	for (auto entity_obj : EntityList){
		delete entity_obj;
	}
	EntityList.clear();

	for (auto spawner_obj : spawner_list){
		delete spawner_obj;
	}
	spawner_list.clear();

	for (auto sound_obj : sound_list){
		delete sound_obj;
	}
	sound_list.clear();

	game->setMultiplier(1);
	game->setHiScore();
	game->setScore(0);
	game->setGameOver(true);

	this->game->popScene();
}

void SceneGame::gameOverCountdown(){
	if (player_dead){
		gameOver_wait--;
	}
	if (gameOver_wait == 0){
		gameOver();
	}
}

Player* SceneGame::getPlayer(){
	return player;
}

void SceneGame::playerKilled(){
	player_dead = true;
	playSound("player_destroyed");
}

void SceneGame::bossDefeated(){
	boss_summoned = false;
	boss_ticks ++;
}

void SceneGame::summonBoss(){
	if (boss_ticks % boss_period == boss_period - 1 && !boss_summoned){
		boss_summoned = true;
		makeBoss();
	}
}

void SceneGame::playSound(std::string sound_buff){
	sf::Sound* newSound = new sf::Sound(
		SoundPlayer::instance()->getRef(sound_buff));
	newSound->play();
	sound_list.push_back(newSound);
}

void SceneGame::checkSounds(){
	for (int i = sound_list.size() - 1; i >= 0; --i){
		if (sound_list[i]->getStatus() != sf::Sound::Status::Playing){
			delete sound_list[i];
			sound_list[i] = nullptr;
			sound_list.erase(sound_list.begin() + i);
		}
	}
}