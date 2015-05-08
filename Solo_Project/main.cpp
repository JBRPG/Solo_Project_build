#include <iostream>

#include <time.h>
#include <cstdio>
#include <cstdlib>

#include "game.hpp"
#include "sceneGame.hpp"

using namespace std;

int main(){

	// Let's start out game

	srand(time(NULL));
	Game game;
	game.pushScene(new SceneGame(&game));
	game.gameLoop();

	return 0;
}