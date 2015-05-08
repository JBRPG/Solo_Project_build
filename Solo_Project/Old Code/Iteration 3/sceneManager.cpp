#include "sceneManager.hpp"

#include <iostream> // for testing purposes


// Default constructor: initalize grid with nullptrs and 0

gridContainer::gridContainer(){
	for (auto i = 0; i < nGridX; ++i){
		for (auto j = 0; j < nGridY; ++j){
			for (auto k = 0; k < nMaxEntities; ++k){
				cell[i][j].items[k] = nullptr;
			}
			cell[i][j].nEntities = 0;
		}
	}
}

gridContainer::~gridContainer(){

}

void gridContainer::append(int x, int y, Entity* e){
	if ((x >= 0) && (x < nGridX) && (y >= 0) && (y < nGridY)){
		if (cell[x][y].nEntities < nMaxEntities)
			cell[x][y].items[cell[x][y].nEntities++] = e;
	}
}

void gridContainer::reset(){
	for (auto i = 0; i < nGridX; ++i){
		for (auto j = 0; j < nGridY; ++j){
			cell[i][j].nEntities = 0;
		}
	}
}

sceneContainer::sceneContainer(sf::RenderWindow& w):
  sGrid {new gridContainer()},
  sGridSize { gViewSize.x / sGrid->nGridX, gViewSize.y / sGrid->nGridY},
  sScreen { sf::FloatRect {-20.0f, -20.0f, static_cast <float>(gViewSize.x + 20),
                        static_cast <float>(gViewSize.y + 20) } },
  sBuffer{ sf::FloatRect {-sBufferSize, -sBufferSize,
                         gViewSize.x + sBufferSize,
                         gViewSize.y + sBufferSize} }
{

}

/* All entities are pushed to the entitiy list,
   which consist of active entities within the screen and buffer zones
*/

void sceneContainer::addEntity(std::unique_ptr<Entity> p){
	sEntityList.push_back(std::move(p));
}

/*
  Checks if a given entry is on screen, in buffer region but not on screen,
  or off the buffer region. The entity should be destroyed if outside buffer zone.
*/
StatusID sceneContainer::onScreen(const sf::Vector2f& pos){
	if ((pos.x >= sScreen.left) && (pos.x >= sScreen.width) &&
		(pos.y >= sScreen.top) && (pos.y >= sScreen.height)){
		return StatusID::ONSCREEN;
	}
	if ((pos.x >= sBuffer.left) && (pos.x >= sBuffer.width) &&
		(pos.y >= sBuffer.top) && (pos.y >= sBuffer.height)){
		return StatusID::ONBUFFER;
	}
	return StatusID::DESTROYED;
}

/*
  collisionCheck returns the number of checks made
*/
int sceneContainer::collisionCheck(){

	int checkCount = 0;

	for (auto i = 0; i < sGrid->nGridX; ++i){
		for (auto j = 0; j < sGrid->nGridY; ++j){
			// Now we compare entity lists for collision detection
			// between unique entities
			for (auto p = 0; p < sGrid->cell[1][j].nEntities; ++p){

				// We will do the a vs b comparison

				Entity* a{ sGrid->cell[i][j].items[p] }; //for readability as Entity a
				if ( (a->getEntityType() == EntityType::PLAYER) ||
					 (a->getEntityType() == EntityType::ENEMY) || 
					 (a->getEntityType() == EntityType::TERRAIN)  ){
					for (auto q = 0; q < sGrid->cell[1][j].nEntities; ++q){
						if (q != p){ // We do not check the same entity

							Entity* b{ sGrid->cell[i][j].items[q] }; // for readability as Entity b
							if

						// We now check between different types of entities
						// To reduce further amounts of checks,
						// we limited the amount of interactions to specific types of entities
						// Player collides with either enemy or enemy bullet
						// Enemy collides with either player bullet or terrain
						// Terrain collides with either enemy bullet, player bullet, or player

						( (a->getEntityType() == EntityType::PLAYER &&
			    (b->getEntityType() == EntityType::ENEMY || b->getEntityType() == EntityType::BULLET_ENEMY)) ||
						  (a->getEntityType() == EntityType::ENEMY  &&
				(b->getEntityType() == EntityType::BULLET_PLAYER || b->getEntityType() == EntityType::TERRAIN)) ||
				          (a->getEntityType() == EntityType::TERRAIN &&
				(b->getEntityType() == EntityType::BULLET_ENEMY || b->getEntityType() == EntityType::BULLET_PLAYER ||
				b->getEntityType() == EntityType::PLAYER))
				           ){
								checkCount++;
								if (a->getAABB().intersects(b->getAABB())){
									if (a->getEntityType() == EntityType::PLAYER){
										// Player vs enemy bullet
										if (b->getEntityType() == EntityType::BULLET_ENEMY){

											// Both A and B get hit
											// for now lets have both player and eney bullet be destroyed
											// the code will be replaced with hit once HP use is established
											a->setStatus(StatusID::DESTROYED);
											b->setStatus(StatusID::DESTROYED);
										}else{
											// Else player vs enemy
											a->setStatus(StatusID::DESTROYED);
											b->setStatus(StatusID::DESTROYED);
											}
									} else 
								if (a->getEntityType() == EntityType::ENEMY){
										// Enemy vs Player
										if (b->getEntityType() == EntityType::BULLET_PLAYER){

											// Again, for early build purposes,
											// set both entities to be destoryed.
											// Will refactor the code once HP use is established
											a->setStatus(StatusID::DESTROYED);
											b->setStatus(StatusID::DESTROYED);

										}
										else {
											// Enemy vs terrain
											a->setStatus(StatusID::DESTROYED);
										}
								}
								else { // a's entity type is terrain
									   // For now, the terrain is invincible
									   // code will be refactored to create destoryable terrain
									b->setStatus(StatusID::DESTROYED);
								  }
								}
							}
						}
					}
				}
			}
		}
	}

	return checkCount;
}


/* Checks local coordinates, and expects such as input.
 * Returns number of non-player entities
*/
unsigned int sceneContainer::update(const float dt){

	sGrid->reset(); // reset screen grid
	sEnemyCount = 0;
	sTerrainCount = 0;

	// For testing
	bool vulnerability = true;

	// First: update each managed entry
	for (auto& i : sEntityList){
		sf::Vector2f pos;

		pos = i->update(dt);

		// The player shall not leave the screen
		if (i->getEntityType() == EntityType::PLAYER){
			if (pos.x < sScreen.left + 15) pos.x = sScreen.left + 15;
			else if (pos.x > sScreen.width - 15) pos.x = sScreen.width - 15;

			if (pos.y < sScreen.top + 15) pos.y = sScreen.top + 15;
			else if (pos.y > sScreen.height - 15) pos.y = sScreen.height - 15;

			i->setPosition(pos);
		}

		// On basis of updated position, let's update entity's status
		if (i->getStatus() != StatusID::EXPLODING && i->getStatus() != StatusID::DESTROYED)
			i->setStatus(StatusID::ONSCREEN);

		/*
		 * Now add entities to the screen grid, based on the position of their AABB's
		 * 4 corner points; and update enemy lists. An enemy could eb registered
		 * to multiple cells in the grid, based on the position of its AABB
		 */
		if (i->getStatus() == StatusID::ONSCREEN){
			Entity* e = i.get();
			sf::FloatRect box{e->getAABB()};
			sf::Vector2i  topleft{ static_cast<int>(box.left / sGridSize.x), static_cast<int>(box.top / sGridSize.y) };
			sf::Vector2i  topright{ static_cast<int>( (box.left + box.width) / sGridSize.x), static_cast<int>(box.top / sGridSize.y) };
			sf::Vector2i  botleft{ static_cast<int>(box.left / sGridSize.x), static_cast<int>((box.top + box.height) / sGridSize.y) };
			sf::Vector2i  botright{ static_cast<int>((box.left + box.width) / sGridSize.x),
				                    static_cast<int>((box.top + box.height) / sGridSize.y) };

			sGrid->append(topleft.x, topleft.y, e);
			if (topright != topleft) sGrid->append(topright.x, topright.y, e);
			if ((botleft != topleft) && (botleft != topright)) sGrid->append(botleft.x, botleft.y, e);
			if ((botright != botleft) && (botright != topleft) && (botright != topright)) sGrid->append(topright.x, topright.y, e);

			if (i->getEntityType() == EntityType::ENEMY) sEnemies[sEnemyCount++] = e;
			if (i->getEntityType() == EntityType::TERRAIN) sTerrain[sTerrainCount++] = e;


		}

	}

	// Okay now do collision checks

	if (vulnerability) collisionCheck();

	/*
	* Finally, let's go through the updated list of entities and
	* remove those that are destoryed. Then return the number of
	* not-destoryed, not-exploding enemies/terrain on scene
	*/

	int entityCount = 0;

	sEntityList.remove_if([](const std::unique_ptr<Entity>& p)
	    {return p->getStatus() == StatusID::DESTROYED; });
	for (auto& i : sEntityList){
		if ((i->getEntityType() == EntityType::ENEMY || i->getEntityType() == EntityType::TERRAIN)
			&& i->getStatus() != StatusID::EXPLODING) ++entityCount;
	}
	return entityCount;
}

void sceneContainer::render(sf::RenderTarget& t, sf::RenderStates s){

	// Draw the entities that are on screen or exploding
	for (auto& i : sEntityList){
		if ((*i).getStatus() == StatusID::ONSCREEN ||
			(*i).getStatus() == StatusID::EXPLODING)
		{	
			t.draw(*i, s);
		}

		/*
		   This Section of code is supposed to draw the entities on the game screen.
		   However, it does not draw. on top of the background.

		  
		   That is because I have not propetly set up the renderstate.
		   I have got the sprites to draw without the render state
		   based on current view.
		*/

	}
}