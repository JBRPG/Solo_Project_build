#include <vector>

#include <SFML\Graphics.hpp>

#include "state_game.hpp"
//#include "entity.hpp"

StateGame::StateGame(Game* game){
	
	this->game = game;

	// Set up the various views
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	this->background1View.setSize(pos);
	this->background2View.setSize(pos);
	this->gameView.setSize(pos);
	this->playerView.setSize(pos);
	this->hudView.setSize(pos);
	pos *= 0.5f;
	this->background1View.setCenter(pos);
	this->background2View.setCenter(pos);
	this->gameView.setCenter(pos);
	this->playerView.setCenter(pos);
	this->hudView.setCenter(pos);


	// Then make some sprites
	// Will need to refactor code to load the necessary sprites and textures
	// from a file instead of making long line of sprite-making code

	gamePlayer = new Player(this->game->texmgr.getRef("playerSprite"));

	//*

	//Need to make correct use of vector

	gameEntities.push_back(Entity (sf::Vector2f(400, 600), 300, this->game->texmgr.getRef("metal wall")));
	gameEntities.push_back(Entity (sf::Vector2f(600, 400), 300, this->game->texmgr.getRef("metal wall")));
	//*/

}

void StateGame::draw(const float dt){

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
}

void StateGame::update(const float dt){
	this->gameView.move(0.5,0); // scroll screen test
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
	gamePlayer->move(dirButtons);

}