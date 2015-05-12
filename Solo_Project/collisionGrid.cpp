#include "collisionGrid.hpp"

collisionGrid::collisionGrid(){
	for (auto i = 0; i < slicesX; ++i){
		for (auto j = 0; j < slicesY; ++j){
			for (auto k = 0; k < maxEntities; ++k){
				cell[i][j].items[k] = nullptr;
			}
			cell[i][j].nEntities = 0;
		}
	}


}

void collisionGrid::append(int x, int y, Entity* e){
	if ((x >= 0) && (x < slicesX) && (y >= 0) && (y < slicesY)){
		if (cell[x][y].nEntities < maxEntities)
			cell[x][y].items[cell[x][y].nEntities++] = e;
	}
}

void collisionGrid::reset(){
	for (auto i = 0; i < slicesX; ++i){
		for (auto j = 0; j < slicesY; ++j){
			cell[i][j].nEntities = 0;
		}
	}
}

void collisionGrid::setOrigin(int _left, int _top){
	left = _left;
	top = _top;
}

sf::Vector2i collisionGrid::getOrigin(){
	return sf::Vector2i(left, top);
}

void collisionGrid::setSize(int _width, int _height){
	width = _width;
	height = _height;
}

sf::Vector2i collisionGrid::getSize(){
	return sf::Vector2i(width, height);
}


sf::IntRect collisionGrid::getCollisionRect(){
	return sf::IntRect(sf::Vector2i(left, top), sf::Vector2i(width, height));
}