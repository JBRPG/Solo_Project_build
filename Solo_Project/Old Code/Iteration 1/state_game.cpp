#include <SFML\Graphics.hpp>

#include "state_game.hpp"
#include "game.hpp"
#include "player.hpp"

StateGame::StateGame(Game* game){
	
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	this->hudView.setSize(pos);
	this->gameView.setSize(pos);
	pos *= 0.5f;
	this->hudView.setCenter(pos);
	this->gameView.setCenter(pos);

}

void StateGame::draw(const float dt){

	this->game->window.setView(this->gameView);

	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);
	this-> game->window.draw(this->game->playerSprite);
}

void StateGame::update(const float dt){

}

void StateGame::handleInput(){
	sf::Event event;

	while (this->game->window.pollEvent(event)){

		if (event.type == sf::Event::Closed){
			game->window.close();
		}

		bool dirButtons[4] = {false, false, false, false};
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
}