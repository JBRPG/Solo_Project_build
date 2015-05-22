#include <iostream>

#include "enemy.hpp"




void Enemy::update(float dt){
	ticks++;
	// The enemy remains stationary if no movement provided
	if (this->myMovement != NULL)
		updateMovement(*myMovement);

	updateWeapon(*myWeapon);

}


void Enemy::updateMovement(Movement& entityMove){
	
	entityMove.update(*this);
}

void Enemy::updateWeapon(Weapon& weapon){
	weapon.update(*this);
}

void Enemy::collideWith(Entity &other){

	if (Player* player = dynamic_cast<Player*>(&other)){

		destroyEnemy();
	}
	else if (Bullet* bullet = dynamic_cast<Bullet*>(&other)) {
		if (bullet->getEnemyShot()) return;

		health--;

		if (health <= 0){
			destroyEnemy();
		}
		else {
			myScene->playSound("enemy_hit");
		}
	}
}

void Enemy::destroyEnemy(){
	myScene->storeRemovedEntity(this);
	myScene->game->increaseScore(1);
	myScene->playSound("enemy_destroyed");
	Explosion* explode = new Explosion
		(TextureManager::instance()->getRef("explodeAnimate"), sf::IntRect(0,0,80,80), 3);
    explode->setSpawnTime(1.0f);
	explode->setPosition(this->getPosition().x - this->getGlobalBounds().width / 2,
		this->getPosition().y);

	myScene->addEntity(explode); // you have to actually add the entity because
	                             // storeAdded list has already been checked during update
}

EnemyTemplate::EnemyTemplate(SceneGame* scene, std::string tex, int hp, float _speed, bool invincible, sf::Vector2f pos):
myScene(scene), strTex(tex), health(hp), speed(_speed), invincible(invincible), spawnPos(pos)

{}


// There is only game scene so we do not want to delete it
EnemyTemplate::~EnemyTemplate(){
	myScene = nullptr;
}