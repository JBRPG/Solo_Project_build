#include "player.hpp"

#include "pickup.hpp"
#include "soundPlayer.hpp"
#include "terrain.hpp"
#include "explosion.hpp"


void Player::update(float dt){
	this->movePlayer();
	this->shootPlayer(dt);
	this->checkHealthPlayer();
	Entity::update(dt);

}


/* 
   For all entity comparisons, we will use dynamic_cast
   with the reference to Entity object - Entity& other
   to keep code cleaner

   dynamic_cast to a pointer type returns NULL if the cast fails
   dynamic_cast to a reference type would throw an exception on failure
*/

void Player::collideWith(Entity& other){
	
	// Check if the entity is Enemy
	if (Enemy* enemy = dynamic_cast<Enemy*>(&other)){
		// Take damage from enemy
		// unless invincible
		--this->health;
	}
	// Check if the entity is Bullet
	else if (Bullet* bullet = dynamic_cast<Bullet*>(&other)){
		if (!bullet->getEnemyShot()) return;

		// Take damage from bullet,
		// unless invincible
		--this->health;
	}
	else if (Pickup* pickup = dynamic_cast<Pickup*> (&other)){
		myScene->playSound("weapon_get");
		this->setWeapon(pickup->giveWeapon());
		this->myScene->game->increaseMultiplier();
	}


	else if (Terrain* land = dynamic_cast<Terrain*> (&other)){
		health = 0;
	}
}


/*
  For barebones purposes,
  we only focus on movement through keyboard arrows
*/
void Player::movePlayer(){

	float sqrt2 = 1.41;
	bool dirArray[4] = { false, false, false, false };

	// The following firections are in order:
	// left, up, right, down

	if (Input::instance()->pressKeybutton(sf::Keyboard::Left))
		dirArray[0] = true;
	if (Input::instance()->pressKeybutton(sf::Keyboard::Up))
		dirArray[1] = true;
	if (Input::instance()->pressKeybutton(sf::Keyboard::Right))
		dirArray[2] = true;
	if (Input::instance()->pressKeybutton(sf::Keyboard::Down))
		dirArray[3] = true;

	sf::Vector2f velocity{ 0, 0 };

	velocity.x = this->speed * (dirArray[2] - dirArray[0]);
	velocity.y = this->speed * (dirArray[3] - dirArray[1]);

	if ((dirArray[0] || dirArray[2]) && (dirArray[1] || dirArray[3])){

		velocity.x /= sqrt2;
		velocity.y /= sqrt2;
	}

	// When I added the scroll speed to the move function,
	// I expected that the player ship would look like it stands still
	// however, the framerate is not constant and often below 60fps,
	// which causes the scrolling to go faster than the player

	this->move(velocity + myScene->getScrollSpeed());
	bank_ship();

	// keep player from going out of bounds
	sf::Vector2f tl_window_view = myScene->game->window.mapPixelToCoords(
		sf::Vector2i(0,0));
	sf::Vector2f br_window_view = myScene->game->window.mapPixelToCoords(
		sf::Vector2i(myScene->game->window.getSize().x, myScene->game->window.getSize().y));

	if (this->getGlobalBounds().left < tl_window_view.x){
		this->setPosition(tl_window_view.x + this->getGlobalBounds().width / 2,
			this->getPosition().y);
	}
	if (this->getPosition().x >= br_window_view.x
		- this->getGlobalBounds().width/2){
		this->setPosition(br_window_view.x - this->getGlobalBounds().width / 2,
			this->getPosition().y);
	}
	if (this->getGlobalBounds().top < tl_window_view.y){
		this->setPosition(this->getPosition().x,
			tl_window_view.y + this->getGlobalBounds().height / 2);
	}
	if (this->getPosition().y >= br_window_view.y
		- this->getGlobalBounds().height / 2){
		this->setPosition(this->getPosition().x,
			br_window_view.y - this->getGlobalBounds().height / 2);
	}
}


void Player::shootPlayer(float dt){

	// looks like playing the sound from the instance class alone may not produce sound.
	// may end up having to implement sound objects in derived entity objects
	// so the sounds can be played


	
	myWeapon->update(*this);

}

void Player::checkHealthPlayer(){
	if (this->health <= 0){
		myScene->playerKilled();
		myScene->storeRemovedEntity(this);

		Explosion* explode = new Explosion
			(TextureManager::instance()->getRef("explodePlayer"), sf::IntRect(0, 0, 64, 32), 10);
		explode->setSpawnTime(1.0f);
		explode->setPosition(this->getPosition().x - this->getGlobalBounds().width / 2,
			this->getPosition().y);

		myScene->addEntity(explode); // you have to actually add the entity because
		// storeAdded list has already been checked during update


	}
}

void Player::bank_ship(){
	bool bank_arr[2] = { false, false };

	if (Input::instance()->pressKeybutton(sf::Keyboard::Up))
		bank_arr[0] = true;
	if (Input::instance()->pressKeybutton(sf::Keyboard::Down))
		bank_arr[1] = true;

	if (bank_arr[0] && !bank_arr[1]){
		bank_ticks = bank_ticks > (-2 * bank_delay) ? bank_ticks - 1 : -2 * bank_delay;
	}
	else if (!bank_arr[0] && bank_arr[1]){

		bank_ticks = bank_ticks < (2 * bank_delay) ? bank_ticks + 1 : 2 * bank_delay;
	}
	else {
		if (bank_ticks > 0){
			bank_ticks--;
		}
		else if (bank_ticks < 0){
			bank_ticks++;
		}
	}

	int frame_val = (bank_ticks / bank_delay) + middle_frame; // will eventually add in the middle value

	// change frame based on value
	this->setTextureRect(sf::IntRect(getTextureRect().width * frame_val, 0, getTextureRect().width, getTextureRect().height));
}