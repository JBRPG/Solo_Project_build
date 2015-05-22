#include "sceneTitle.hpp"

#include <sstream>

#include "game.hpp"
#include "sceneGame.hpp"
#include "textureManager.hpp"
#include "soundPlayer.hpp"


SceneTitle::SceneTitle(Game* game)
{

	player_destroyed = sf::Sound(SoundPlayer::instance()->getRef("player_destroyed"));

	this->game = game;

	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	titleView.setSize(pos);
	pos *= 0.5f;
	titleView.setCenter(pos);

	font.loadFromFile("media/arial.ttf");
	controls = sf::Text("Controls:\n Arrows - Move   Space - Shoot\n\n\nPress Enter to begin game.", font, 30);
	controls.setPosition(200, 400);
	hiscore = sf::Text("Hi Score: " + game->getHiScore(), font, 30);
	hiscore.setPosition(500, 0);

	title = sf::Sprite(TextureManager::instance()->getRef("title"));
	title.setPosition(200, 100);
}

void SceneTitle::draw(const float dt){
	game->window.setView(titleView);
	game->window.draw(title);
	game->window.draw(controls);
	game->window.draw(hiscore);

}


void SceneTitle::update(const float dt){
	std::stringstream hiscoreDisplay;
	hiscoreDisplay << " Hi Score: " << game->getHiScore();
	std::string hiscoreStr = hiscoreDisplay.str();
	hiscore.setString(hiscoreStr);
	play_playerDefeat();


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
		player_destroyed.play();
		game->setAtStartScreen(true);
	}
}