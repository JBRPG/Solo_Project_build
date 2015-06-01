#include "boss.hpp"

#include "weapon.hpp"
#include "movement.hpp"
#include "sceneGame.hpp"
#include "explosion.hpp"

Boss::Boss(SceneGame* scene, std::string tex, int hp, float _speed, bool _invincible,
	sf::Vector2f pos, Weapon* weapon, Movement* movement,
	std::string bossName, std::vector<Weapon*> _weapon_list, std::vector<int> hp_phase, int boss_wait):
	Enemy(scene, tex, hp, _speed, _invincible, pos, weapon, movement),
	boss_name(bossName), weapons(_weapon_list), boss_phase_hp(hp_phase), intro_timer(boss_wait),
	intro_time_set(boss_wait)
{

}

Boss::~Boss(){

	for (auto _weapon : weapons){
		delete _weapon;
	}
}

void Boss::update(float dt){
	intro_timer = intro_timer > 0 ? intro_timer - 1 : 0;
	if (intro_timer <= 0){
		invincible = false;
		setColor(sf::Color(255, 255, 255, 255));
		move(myScene->getScrollSpeed());
		updateWeapon(*myWeapon);
		updateMovement(*myMovement);
	}
	else{
		invincible = true;
		sf::Uint8 alpha_appear = 255 * ( float(intro_time_set - intro_timer ) / float(intro_time_set));
		setColor(sf::Color(0, 255, 0, alpha_appear));
		move(-myScene->getScrollSpeed().x, 0);
	}


}


void Boss::updateMovement(Movement& movement){
	Enemy::updateMovement(movement);
}

void Boss::updateWeapon(Weapon& weapon){
	Enemy::updateWeapon(weapon);
}

void Boss::collideWith(Entity& other){
	// Will fill in its own set so boss can die in unique explosion


	if (Player* player = dynamic_cast<Player*>(&other)){

	}
	else if (Bullet* bullet = dynamic_cast<Bullet*>(&other)) {
		if (bullet->getEnemyShot() || invincible) return;

		health--;
		checkPhase();
		if (health <= 0){
			destroyBoss();
		}
		else {
			myScene->playSound("enemy_hit");
		}
	}
}

void Boss::destroyBoss(){

	myScene->storeRemovedEntity(this);
	myScene->game->increaseScore(1000);
	myScene->playSound("enemy_destroyed");
	myScene->bossDefeated();
	Explosion* explode = new Explosion
		(TextureManager::instance()->getRef("explodeAnimateBig"), sf::IntRect(0, 0, 125, 128), 8);
	explode->setSpawnTime(1.0f);
	explode->setPosition(this->getPosition().x - this->getGlobalBounds().width / 2,
		this->getPosition().y);

	myScene->addEntity(explode); // you have to actually add the entity because
	// storeAdded list has already been checked during update
}

void Boss::checkPhase(){
	if (phase < boss_phase_hp.size() - 1 && health <= boss_phase_hp[phase + 1]){
		// replace current weapon with new one
		phase++;
		delete myWeapon;
		myWeapon = new Weapon(*weapons[phase]);

	}
}