#include "pickup.hpp"

#include "player.hpp"

void Pickup::collideWith(Entity& other){
	if (Player* player = dynamic_cast<Player*> (&other)){
		myScene->storeRemovedEntity(this);
	}
}

Weapon* Pickup::giveWeapon(){
	return new Weapon(*myWeapon);
}