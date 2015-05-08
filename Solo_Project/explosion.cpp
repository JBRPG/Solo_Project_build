#include "explosion.hpp"

 void Explosion::update(float dt){
	 spawnTime -= dt;
	 checkExplosion();
}

 void Explosion::checkExplosion() {
	 if (spawnTime <= 0)
		 myScene->storeRemovedEntity(this);
 };