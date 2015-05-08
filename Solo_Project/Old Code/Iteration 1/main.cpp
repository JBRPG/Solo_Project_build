#include <iostream>

#include "game.hpp"
#include "state_game.hpp"

using namespace std;

int main(){

	/* Solo Project in the works*/

	Game game;

	game.pushState(new StateGame(&game));
	game.gameLoop();

	return 0;
}