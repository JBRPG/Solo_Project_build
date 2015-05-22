#include "bullet.hpp"

#include "terrain.hpp"


/*
For all entity comparisons, we will use dynamic_cast
with the reference to Entity object - Entity& other
to keep code cleaner

dynamic_cast to a pointer type returns NULL if the cast fails
dynamic_cast to a reference type would throw an exception on failure
*/

void Bullet::collideWith(Entity& other){

	if (Player* player = dynamic_cast<Player*>(&other)){
		if (!enemyShot) return;

		// destroy the bullet
		// player health reduction taken care of
		// in Player's code

		myScene->storeRemovedEntity(this);
	}
	else if (Enemy* enemy = dynamic_cast<Enemy*>(&other)){
		if (enemyShot) return;

		// destroy bullet
		// enemy health reduction in its own code

		myScene->storeRemovedEntity(this);
	}


	else if (Terrain* land = dynamic_cast<Terrain*> (&other)){
		myScene->storeRemovedEntity(this);
	}
}


void Bullet::update(float dt){

	// Since math functions take radians for sine and cosine,
	// we had to covnert from degrees to radians through the variables below

	float pi = 3.14;
	float deg_to_rad = pi / 180.0f;

	// Move the bullet with the given velocity (speed and rotation angle)
	this->move(speed * cos(rotation * deg_to_rad),
		       speed * sin(rotation * deg_to_rad));


}

// bullet template section

BulletTemplate::BulletTemplate(const BulletTemplate& source){
	// shallow copy-able
	texString = source.texString;
	hp = source.hp;
	speed = source.speed;
	invincibility = source.invincibility;
	rotation = source.rotation;
}

// overloaded assignment operator
BulletTemplate& BulletTemplate::operator= (const BulletTemplate& source){
	// self assignment check
	if (this == &source) return *this;

	texString = source.texString;
	hp = source.hp;
	speed = source.speed;
	invincibility = source.invincibility;
	rotation = source.rotation;

	return *this;
}

BulletTemplate::~BulletTemplate(){

}