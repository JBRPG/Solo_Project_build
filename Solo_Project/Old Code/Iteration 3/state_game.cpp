#include <vector>

#include <SFML\Graphics.hpp>

#include "state_game.hpp"
//#include "entity.hpp"

StateGame::StateGame(Game& game):
mLevel(game){
	
	this->game = &game;

	


}

void StateGame::draw(const float dt){

	/*

	// This is currently being refactored 

	this->game->window.setView(this->background1View);

	// All objects drawn within background1View will be affected
	// by background1View's active window
	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);

	// Same thing will happen with gameView and so on
	this->game->window.setView(this->gameView);
	this->game->window.draw(this->gameEntities.at(0).getSprite());
	this->game->window.draw(this->gameEntities.at(1).getSprite());


	this->game->window.setView(this->playerView); 
	this->game->window.draw(this->gamePlayer->getSprite());

	*/

	mLevel.render();
}

void StateGame::update(const float dt){
	//this->gameView.move(0.5,0); // scroll screen test

	mLevel.update(dt);

}

void StateGame::handleInput(){
	sf::Event event;

	/* Events should only be used to handle events
	   that does not involve player control
	   such as close window or pause game
	   because they do not respond by framerate, but by order of events
	*/
	while (this->game->window.pollEvent(event)){

		if (event.type == sf::Event::Closed){
			game->window.close();
		}

	}

	/*
	  When you do anything outside the poll events,
	  those actions will always be responsive based on frame limit
	*/

	bool dirButtons[4] = { false, false, false, false };
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		dirButtons[0] = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
		dirButtons[1] = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
		dirButtons[2] = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
		dirButtons[3] = true;
	}
	mLevel.getPlayer().move(dirButtons);

}