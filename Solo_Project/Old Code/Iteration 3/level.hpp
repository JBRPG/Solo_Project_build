#pragma once

#include <queue>
#include <memory>

#include <SFML\Graphics.hpp>

#include "textureManager.hpp"
#include "game.hpp"
#include "globalVariables.hpp"
#include "sceneManager.hpp"
#include "player.hpp"

class Level: sf::NonCopyable {

public:
	Level(Game& game);

	void update(const float dt);
	void render();

	Player getPlayer() { return mPlayer; };
	


private:

	sf::Transform&         getViewTransform() const;

	// Variables from other classes
	sf::RenderWindow&      mWindow;


	// Level's variables

	sf::View               mViewBG1;
	sf::View               mViewBG2;
	sf::View               mViewGame;
	sf::View               mViewPlayer;
	sf::View               mViewHUD;

	sf::Vector2f           mViewOffset;
	sf::Vector2f           mLevelSize;
	sf::Vector2f           mScrTopLeft;

	sf::Sprite             mBackground;

	//TextureManager         mTex; // A dummy variable 
	Game&                   mGame;
	sceneContainer         mScene;
	Player                 mPlayer;
	std::unique_ptr<Entity> mPlayerEntity;
	

};