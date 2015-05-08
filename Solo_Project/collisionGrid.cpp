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