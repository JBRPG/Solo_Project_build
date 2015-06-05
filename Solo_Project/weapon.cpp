#include "weapon.hpp"

#include "bullet.hpp"

/*
  Construct a weapon by having a vector of bullets with a given keyword
  A weapon will have a given delay time between shooting with the weapon
  A specific cooldown will be used based on the given keyword.


*/

// this is for player only
Weapon::Weapon():
keyword("single"), shootCooldownSet(120)
{
	std::vector<BulletTemplate*> bullets;
	bullets.push_back(new BulletTemplate("bulletPlayer",1, 30, false, 0));
	bullet_list = bullets;
}

Weapon::Weapon(std::vector <BulletTemplate*> bullets, std::string key_name, int delay, std::vector<int> key_params) :
bullet_list(bullets), keyword(key_name), shootCooldownSet(delay)
{

	// If none of the other keywords match, the program will close

		if (keyword == "rapid_player" || keyword == "rapid_enemy"){
			rapidWaitSet = key_params[0];
			rapidDurationSet = key_params[1];
			return;
		}

		if (keyword == "hold_player"){
			holdDurationSet = key_params[0];
			return;
		}

		if (keyword == "sequence_enemy"){
			sequenceDelaySet = key_params[0];
			return;
		}

		if (keyword == "sequence_multi_enemy"){
			sequenceDelaySet = key_params[0];
			bullets_fired = key_params[1];
			return;
		}
		exit(1);
}

Weapon::Weapon(std::vector <BulletTemplate*> bullets, std::string key_name, int delay) :
bullet_list(bullets), keyword(key_name), shootCooldownSet(delay)
{

	if (keyword != "single") {
		exit(1);
	}
}

// copy constructor
Weapon::Weapon(const Weapon& source){

	shootCooldownSet = source.shootCooldownSet;
	rapidWaitSet = source.rapidWaitSet;
	rapidDurationSet = source.rapidDurationSet;
	sequenceDelaySet = source.sequenceDelaySet;
	sequence_idx = source.sequence_idx;
	holdDurationSet = source.holdDurationSet;
	spawnTime = source.spawnTime;
	keyword = source.keyword; 
	enemydidShoot = source.enemydidShoot;
	bullets_fired = source.bullets_fired;

	for (auto bullet : source.bullet_list){
		bullet_list.push_back(new BulletTemplate(*bullet));
	}

}

// assignment operator
Weapon& Weapon::operator=(const Weapon& source){
	// self checking
	if (this == &source) return *this;

	// first delete dynamic objects

	for (auto bullet : bullet_list){
		delete bullet;
	}
	bullet_list.clear();

	// then initalize

	///*
	shootCooldownSet = source.shootCooldownSet;
	rapidWaitSet = source.rapidWaitSet;
	rapidDurationSet = source.rapidDurationSet;
	sequenceDelaySet = source.sequenceDelaySet;
	sequence_idx = source.sequence_idx;
	holdDurationSet = source.holdDurationSet;
	spawnTime = source.spawnTime;
	keyword = source.keyword;
	enemydidShoot = source.enemydidShoot;

	//bullet_list = source.bullet_list;

	for (auto bullet : source.bullet_list){
		bullet_list.push_back(new BulletTemplate(*bullet));
	}

	//*/

	return *this;
}

Weapon::~Weapon(){
	for (BulletTemplate* bullet : bullet_list){
		delete bullet;
	}
}

void Weapon::update(Entity& shooter){

	lookupShoot(shooter, this->keyword);

}

Weapon* Weapon::replaceWeapon(Weapon* newWeapon){
	return new Weapon(*newWeapon);
}

void Weapon::lookupShoot(Entity& shooter, std::string name){

	static std::map < std::string, void (Weapon::*)(Entity&) > table
	{
		{ "single", &Weapon::singleShot},
		{ "rapid_player", &Weapon::rapidPlayer},
		{ "rapid_enemy", &Weapon::rapidEnemy },
		{ "hold_player", &Weapon::holdPlayer },
		{ "sequence_enemy", &Weapon::sequenceEnemy },
		{ "sequence_multi_enemy", &Weapon::sequenceMultiEnemy }
	};

	auto entry = table.find(name);
	if (entry != table.end()){
		(this->*(entry->second)) (shooter);
	}
	else {

		std::cerr << "Cannot find movement with the name " << name << std::endl;
		std::exit(1);
	}

}

// When an entity shoots bullets, we will find out if its a player or enemy
// to determine who shot them

// Shoot all bullets at once
void Weapon::shootBullets(Entity& shooter){
	
	if (Enemy* enemy = dynamic_cast<Enemy*> (&shooter)){
		enemydidShoot = true;
	}
	else if (Player* player = dynamic_cast<Player*> (&shooter)){
		enemydidShoot = false;
	}

	for (auto bullet_data : bullet_list){
		Bullet* bullet_p = new Bullet(
			TextureManager::instance()->getRef(bullet_data->getTex()),
			bullet_data->getHP(),
			bullet_data->getSpeed(),
			bullet_data->getInvincible(),
			enemydidShoot,
			bullet_data->getRotation() + (180 * enemydidShoot));
		int shootFlip = enemydidShoot ? -1 : 1;
		bullet_p->setPosition(shooter.getPosition().x + (shootFlip * shooter.getGlobalBounds().width / 2),
			shooter.getPosition().y);
		shooter.getScene()->storeAddedEntity(bullet_p);
	}

}

// Shoot a single bullet
void Weapon::shootBullet(Entity& shooter, BulletTemplate& bullet){
	if (Enemy* enemy = dynamic_cast<Enemy*> (&shooter)){
		enemydidShoot = true;
	}
	else if (Player* player = dynamic_cast<Player*> (&shooter)){
		enemydidShoot = false;
	}
	else {
		return;
	}

	Bullet* bullet_p = new Bullet(
		TextureManager::instance()->getRef(bullet.getTex()),
		bullet.getHP(),
		bullet.getSpeed(),
		bullet.getInvincible(),
		enemydidShoot,
		bullet.getRotation() + (180 * enemydidShoot));
	int shootFlip = enemydidShoot ? -1 : 1;
	bullet_p->setPosition(shooter.getPosition().x + (shootFlip * shooter.getGlobalBounds().width / 2),
		shooter.getPosition().y);
	shooter.getScene()->storeAddedEntity(bullet_p);


}

void Weapon::singleShot(Entity& shooter){

	// If entity is player, then cooldown will activate when player holds down fire key
	// otherwise cooldown will reset to 0, to allow rapid pressing.

	if (Player* player = dynamic_cast<Player*> (&shooter)){
		if (Input::instance()->pressKeybutton(sf::Keyboard::Space)){

			if (shootCooldownTime == 0){
				shooter.getScene()->playSound("player_shot");
				shootBullets(shooter);
				shootCooldownTime = shootCooldownSet;
			}

			else {
				shootCooldownTime--;
			}
		}
		else {
			shootCooldownTime = 0;
		}
		
	}

	if (Enemy* enemy = dynamic_cast<Enemy*> (&shooter)){
		if (shootCooldownTime == 0){
			shootBullets(shooter);
			shootCooldownTime = shootCooldownSet;
		}

		else {
			shootCooldownTime--;
		}
	}

}

void Weapon::rapidFire(Entity& shooter){
	if (rapidWait == 0){
		shootBullets(shooter);
		rapidWait = rapidWaitSet;
	}
	else {
		rapidWait--;
	}
}

void Weapon::rapidEnemy(Entity& shooter){
	if (shootCooldownTime == 0){

		if (rapidDuration == 0){
			shootCooldownTime = shootCooldownSet;
			rapidDuration = rapidDurationSet;

		}
		else {
			rapidFire(shooter);
			rapidDuration--;
		}

	}
	else shootCooldownTime--;
}

void Weapon::rapidPlayer(Entity& shooter){
	
	if (Input::instance()->pressKeybutton(sf::Keyboard::Space)){
		if (shootCooldownTime == 0){

			if (rapidDuration == 0){
				shootCooldownTime = shootCooldownSet;
				rapidDuration = rapidDurationSet;

			}
			else {
				rapidFire(shooter);
				rapidDuration--;
			}

		}
		else shootCooldownTime--;
	}
	else {
		shootCooldownTime = 0;
	}
}

void Weapon::holdPlayer(Entity& shoter){}

void Weapon::sequenceFire(Entity& shooter){

	if (sequenceDelayTime == 0){

		if (sequence_idx < bullet_list.size()){
			shootBullet(shooter, *bullet_list[sequence_idx]);
			sequenceDelayTime = sequenceDelaySet;
			sequence_idx++;
		}
		else {
			shootCooldownTime = shootCooldownSet;
			sequence_idx = 0;
		}
	}
	else {
		sequenceDelayTime--;
	}

}


// Will amke a few changes so enemy can shoot multiple bullets at once in sequence

void Weapon::sequenceMultiFire(Entity& shooter){

	// Will polish up area later

	if (sequenceDelayTime == 0){

		if (sequence_idx < bullet_list.size()){
			for (int i = 0; i < bullets_fired; ++i){
				int shoot_idx = (sequence_idx + i) % bullet_list.size();
				shootBullet(shooter, *bullet_list[shoot_idx]);
			}
			sequenceDelayTime = sequenceDelaySet;
			sequence_idx += bullets_fired;
		}
		else {
			shootCooldownTime = shootCooldownSet;
			sequence_idx = 0;
		}
	}
	else {
		sequenceDelayTime--;
	}
}



void Weapon::sequenceEnemy(Entity& shooter){
	if (shootCooldownTime == 0){
			sequenceFire(shooter);
	}
	else shootCooldownTime--;
}

void Weapon::sequenceMultiEnemy(Entity& shooter){
	if (shootCooldownTime == 0){
		sequenceMultiFire(shooter);
	}
	else shootCooldownTime--;
}

std::string Weapon::getKeyword(){
	return keyword;
}

int Weapon::getShotLimit(){
	return shot_limit;
}

void Weapon::setShotLimit(int limit){
	shot_limit = limit;
}

void Weapon::reduceShotCount(int reduce){
	shot_count = shot_count > 0 ? shot_count - reduce : 0;
}