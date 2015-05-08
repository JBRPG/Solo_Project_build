#include "game.hpp"
#include "globalVariables.hpp"
#include "state.hpp"

Game::Game(){
	// initialize render window, view
	gameWindow.create(sf::VideoMode::getDesktopMode(), "John's Solo Project" + gVersion);
	gameView = gameWindow.getDefaultView();
	gameView.setSize(800, 640);
	gameView.setCenter(gameView.getSize().x / 2, gameView.getSize().y / 2);


}

void Game::run(){
	launchState<State>(); // will use a different state later

	// Main Loop
	// OK so this is what happens here. There's only ever one active state,
	// that is on top of the state stack; that needs to run() all the time.
	// But when it comes to drawing, it's important to draw the current stack
	// of States BELOW the active one, so that we can see e.g. the paused
	// game field underneath the pause menu.
	// Then, when all drawings are done, we display the thing.
	// O.c. if we don't need a state drawn when not active (e.g. we don't need
	// to see the main menu in the background of the actual game),
	// then we don't have to; states keep track their own mIsActive member.

	while (!rStateStack.empty()){

		gameWindow.clear();

		for (const auto& i : rStateStack){
			i->render();
		}

		gameWindow.display();
		(*rStateStack.rbegin())->run();

		// And here: let's call the engine's 'let's apply stack changes' method,
		// so we make sure to run() everything first, THEN change the state (if
		// needed). Thereby we can avoid a situation where a state requests a
		// pop, is popped, and THEN it tries to refer to some own member which
		// has since then been gone.

		executeStackChanges();
	}

}

void Game::executeStackChanges()
{
	// Parameters expected in mStackAction and mStackParam.
	switch (rStackAction) {
	case SAID::POP:
	{
		popState();
		break;
	}
	case SAID::PUSH:
	{
		pushState(rStackParam);
		break;
	}
	case SAID::SWAP:
	{
		swapState(rStackParam);
		break;
	}
	case SAID::RETURN_TO:
	{
		returnToState(rStackParam);
		break;
	}
	case SAID::CLEAR:
	{
		clearStateStack();
		break;
	}
	default: break;
	}

	// Finally, reset stack action requests:
	rStackAction = SAID::NOTHING;
}