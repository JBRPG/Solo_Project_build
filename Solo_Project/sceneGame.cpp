#include "sceneGame.hpp"

#include <sstream>
#include <time.h>
#include <cstdio>
#include <cstdlib>

#include "input.hpp"

#include "spawner.hpp"

#include "pickup.hpp"


SceneGame::SceneGame(Game* game){

	this->game = game; // need this to run the game.

	font.loadFromFile("media/arial.ttf");
	fpsDisplay.setPosition(50, 0);

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

	// setup the bullet patterns

	// Hardcoded for testing purposes

	std::vector<BulletTemplate*> player_weapon;

	// Typical 3-way gun
	player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 15, false, 0));
	player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 15, false, -15));
	player_weapon.push_back(new BulletTemplate("bulletPlayer", 1, 15, false, 15));

	// simple rapid fire

	bullet_Patterns.push_back(player_weapon);

	// Initialize the entities

	// initalize the powerup for test purpose
	powup = new Pickup(this,"pickup", sf::Vector2f(400,200) , new Weapon((bullet_Patterns[0]), "single", 60));
	addEntity(powup);

	// Player will always be initalized at the start of the game

	player = new Player(TextureManager::instance()->getRef("playerSprite"),
		1, 5, false, 3);
	player->setPosition(sf::Vector2f(100, 200));
	player->setWeapon(new Weapon());

	addEntity(player);

	// set up randomization and PCG elements

	// movement list contains 4 types of movement as of now
	movement_select_list = std::vector<bool>(4, false);

}

void SceneGame::draw(float dt){

	// draw the background
	this->game->window.setView(background1View);
	this->game->window.draw(background);
	this->game->window.draw(fpsDisplay);

	// draw the entities

	this->game->window.setView(gameView);

	for (int i = 0; i < getEntitysize(); ++i){
		sf::Sprite* idxSprite = getEntity(i);
		this->game->window.draw(*idxSprite);
	}

}

void SceneGame::update(float dt){
	++scene_ticks;
	///*
	std::stringstream strDisplay;
	strDisplay << "FPS: " << int(1 / dt);

	framerate = strDisplay.str();

	fpsDisplay.setString(framerate);

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


	gameView.move(scrollSpeed);


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
				game->window.getSize().x/gridBox.slicesX,
				game->window.getSize().y/gridBox.slicesY));
	sf::IntRect windowRect = sf::IntRect(0, 0,
		game->window.getSize().x, game->window.getSize().y);
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
				cellRect.left = (game->window.getSize().x) *
					            (float(x) / float(gridBox.slicesX));
				cellRect.top = (game->window.getSize().y) *
					            (float(y) / float(gridBox.slicesY));
				cellRect.width = (game->window.getSize().x) / (gridBox.slicesX);
				cellRect.height = (game->window.getSize().y) / (gridBox.slicesY);


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

	sf::IntRect windowRect = sf::IntRect(0, 0, game->window.getSize().x, game->window.getSize().y);

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

	sf::Vector2i spawn_window_coords = sf::Vector2i(798, randomWindowY);

	sf::Vector2f window_to_map_spawn = game->window.mapPixelToCoords(spawn_window_coords, gameView);


	// will replace movement with semi-randomized system
	Movement* enemy_movement = makeMovement(window_to_map_spawn);

	newSpawn = new Spawner(
		new Weapon(enemy_weapon, "sequence_enemy", 36, { 8 }),
		new Movement(*enemy_movement),
		new EnemyTemplate(this, "enemySprite", 1, 2, false,
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
			sf::Vector2f(-100, 0),
			sf::Vector2f(-100, -100),
			sf::Vector2f(0, -100),
		};

		newMove = new Movement(vertex, waypoints);
	}
	else {
		// make movement based on mathematic formulas
		// I normally do not like using magic numbers
		// but I am using them for test purposes

		if (wordKey == "straight"){
			float angle = float(rand() % 90 + 90);
			newMove = new Movement(wordKey, vertex, { angle });

		}
		else if (wordKey == "circle"){
			float radius = float(rand() % 100 + 21);
			newMove = new Movement(wordKey, vertex, { radius });


		}
		else if (wordKey == "sine"){
			float amplitude = float(rand() % 50 + 1);
			float period = float(rand() % 10 + 1) / 10;
			newMove = new Movement(wordKey, vertex, { amplitude, period });

		}


	}

	return newMove;
}