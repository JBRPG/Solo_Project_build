#include "sceneTitle.hpp"

#include <sstream>

#include "game.hpp"
#include "sceneGame.hpp"
#include "textureManager.hpp"
#include "soundPlayer.hpp"


SceneTitle::SceneTitle(Game* game)
{

	this->game = game;

	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	titleView.setSize(pos);
	pos *= 0.5f;
	titleView.setCenter(pos);

	font.loadFromFile("media/PressStart2P-Regular.ttf");
	controls = sf::Text("Controls:\n Arrows - Move   Space - Shoot\n\nPress Enter to begin game.", font, 16);
	controls.setPosition(200, 400);
	hiscore = sf::Text("Hi Score: " + game->getHiScore(), font, 16);
	hiscore.setPosition(500, 0);

	game_title = new AnimatedParticle(TextureManager::instance()->getRef("title_animated"),
		sf::IntRect(0,0,480,300),180);
	game_title->setPosition(400, 200);
}

void SceneTitle::draw(const float dt){
	game->window.setView(titleView);
	game->window.draw(*game_title);
	game->window.draw(controls);
	game->window.draw(hiscore);

}


void SceneTitle::update(const float dt){
	std::stringstream hiscoreDisplay;
	hiscoreDisplay << " Hi Score: " << game->getHiScore();
	std::string hiscoreStr = hiscoreDisplay.str();
	hiscore.setString(hiscoreStr);
	play_playerDefeat();
	game_title->update(dt);


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

			// press Enter to start game
			else if (event.key.code == sf::Keyboard::Return){
				game->setGameOver(false);
				game->setAtStartScreen(false);
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

void SceneTitle::play_playerDefeat(){
	if (game->getAtStartScreen()) return;
	if (game->getGameOver()){
		game_title->resetAnimation();
		game->setAtStartScreen(true);
	}
}