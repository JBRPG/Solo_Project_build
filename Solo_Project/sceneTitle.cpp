#include "sceneTitle.hpp"


#include "game.hpp"
#include "sceneGame.hpp"
#include "textureManager.hpp"


SceneTitle::SceneTitle(Game* game){

	this->game = game;


	font.loadFromFile("media/arial.ttf");
	controls = sf::Text("Controls:\n Arrows - Move   Space - Shoot\n\n\nPress Space to begin game.", font, 30);
	controls.setPosition(200, 400);

	title = sf::Sprite(TextureManager::instance()->getRef("title"));
	title.setPosition(200, 100);

	// set up the images for the title screen
}

void SceneTitle::draw(const float dt){
	game->window.setView(titleView);
	game->window.draw(title);
	game->window.draw(controls);

}


void SceneTitle::update(const float dt){

}

void SceneTitle::handleInput(){

	sf::Event event;

	while (this->game->window.pollEvent(event)){
		switch (event.type){
		case sf::Event::Closed:
		{
			this->game->window.close();
			break;
		}

		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape){
				this->game->window.close();
			}

			// press fire key (space) to start game
			else if (event.key.code == sf::Keyboard::Space){
				this->game->pushScene(new SceneGame(game));
			}
			break;
		}


		default:
		{
			break;
		}

		}
	}

}