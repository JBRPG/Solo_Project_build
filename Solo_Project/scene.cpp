#include "scene.hpp"

void Scene::addEntity(Entity* entity){;
	EntityList.push_back(entity);
}

void Scene::removeEntity(Entity* Entity){

	/*
	  Since a vector is being used, I want to find some way
	  to remove the entity that is stored inside the Vector
	*/

}

void Scene::cleanEntityList(){
	
	// erase all the entities inside the vector


}

int Scene::getEntitysize(){
	return EntityList.size();
}

Entity* Scene::getEntity(int idx){
	return EntityList.at(idx);
}