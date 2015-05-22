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


SceneGame::SceneGame(Game* game){

	this->game = game; // need this to run the game.


	// setup the collision box
	// gridBox must be larger scale of the game window
	gridBox.setSize(game->window.getSize().x * 1.5,
		            game->window.getSize().y * 1.5);
	// make sure game window is at center of collision box
	
	gridBox.setOrigin(-(int(game->window.getSize().x / 4)),
		              -(int(game->window.getSize().y / 4)));

	font.loadFromFile("media/arial.ttf");
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

	// setup the bullet patterns

	// Hardcoded for testing purposes

	std::vector<BulletTemplate*> player_weapon;

	// Typical 3-way gun
	player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 20, false, 0));
	player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 20, false, -15));
	player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 20, false, 15));

	// simple rapid fire

	bullet_Patterns.push_back(player_weapon);

	// Initialize the entities

	// initalize the powerup for test purpose
	powup = new Pickup(this,"pickup", sf::Vector2f(400,200) , new Weapon((bullet_Patterns[0]), "single", 60));
	addEntity(powup);

	// Player will always be initalized at the start of the game

	player = new Player(TextureManager::instance()->getRef("playerSprite"),
		1, 8, false, 3);
	player->setPosition(sf::Vector2f(100, 200));
	player->setWeapon(new Weapon());

	addEntity(player);

	// set up randomization and PCG elements

	// movement list contains 4 types of movement as of now
	movement_select_list = std::vector<bool>(4, false);

}

void SceneGame::draw(float dt){

	this->game->window.setView(background1View);
	this->game->window.draw(background);
	this->game->window.draw(fpsDisplay);
	this->game->window.draw(score);
	

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

}

void SceneGame::update(float dt){
	++scene_ticks;
	///*
	std::stringstream strDisplay;
	strDisplay << "FPS: " << int(1 / dt);
	framerate = strDisplay.str();
	fpsDisplay.setString(framerate);

	std::stringstream scoreDisplay;
	scoreDisplay << "Score: " << game->getScore();
	curr_score = scoreDisplay.str();
	score.setString(curr_score);


	checkStars();

	makeTerrain();
	spawnTimer(); 

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

	gameOver();


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

// We shall test spanwer creation with fixed variables
Spawner* SceneGame::makeSpawner(){
	Spawner* newSpawn;

	std::vector<BulletTemplate*> enemy_weapon;

	// 2 shot sequence
	enemy_weapon.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, -5));
	enemy_weapon.push_back(new BulletTemplate("bulletEnemy", 1, 10, false, 5));

	
	// this is where you assign the spawner through window coordinates
	// make sure that the initial location is randomized

	int randomWindowY = rand() % 200 + 200;

	sf::Vector2i spawn_window_coords = sf::Vector2i(850, randomWindowY);

	sf::Vector2f window_to_map_spawn = game->window.mapPixelToCoords(spawn_window_coords, gameView);


	// will replace movement with semi-randomized system
	Movement* enemy_movement = makeMovement(window_to_map_spawn);

	newSpawn = new Spawner(
		new Weapon(enemy_weapon, "sequence_enemy", 36, { 8 }),
		new Movement(*enemy_movement),
		new EnemyTemplate(this, "enemySprite", 1, 4, false,
		enemy_movement->getVertex()), {30, 3}, spawn_window_coords);


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
		wordKey = "straight"; // "straight" replaced with "circle" for test purposes
		break;
	case (2) :
		wordKey = "circle";
		break;
	case (3) :
		wordKey = "sine";
		break;
	default:
		wordKey = "waypoint"; // "waypoint" replaced with "sine" for test purposes
		break;
	}

	if (wordKey == "waypoint"){
		// make movement with waypoints

		// initalize Movement

		// right now for testing purposes, we have a predefined waypoint pattern.
		// Will explore more options for creating unique waypoint patterns later.

		std::vector<sf::Vector2f> waypoints = {
			sf::Vector2f(-200, 0),
			sf::Vector2f(-200, -200),
			sf::Vector2f(0, -200),
		};

		newMove = new Movement(vertex, waypoints);
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

void SceneGame::makeTerrain(){
	// make terrain 1/3 of the time
	if (scene_ticks % spawn_time == 0 &&
		rand() % 2 == 0){

		int pow_width = (rand() % 4) + 5;
		int pow_height = (rand() % 4) + 5;
		int terr_width = int(pow(2, pow_width));
		int terr_height = int(pow(2, pow_height));
		bool top_bot = rand() % 2 == 1 ? true : false;

		Terrain* newBlock = new Terrain(TextureManager::instance()->getRef("metal wall"),
			sf::IntRect(0,0,terr_width,terr_height), 100, 0, 1);
		terrain_list.push_back(newBlock);
		addEntity(newBlock);

		sf::Vector2f window_size = game->window.mapPixelToCoords(
			sf::Vector2i(game->window.getSize().x, game->window.getSize().y), gameView);

		newBlock->setPosition(window_size.x + newBlock->getLocalBounds().width / 2,
			(window_size.y * top_bot) - ((2 * (top_bot) - 1) * newBlock->getLocalBounds().height / 2));
		

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

	game->setScore(0);
	game->setMultiplier(1);
	game->setHiScore();
	game->setGameOver(true);

	this->game->popScene();
}

void SceneGame::playerKilled(){
	player_dead = true;
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