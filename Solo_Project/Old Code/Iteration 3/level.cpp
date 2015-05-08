#include "level.hpp"


/*
   To Do: Polish up the other classes before returning to this
   for implementation
*/

Level::Level(Game& game):
mScene{game.window},
mWindow{game.window},
mGame{game},
mPlayerEntity{ new Entity(sf::Vector2f(100, 300),
sf::Vector2f(0.0f, 0.0f),
1,
EntityType::PLAYER,
this->mGame.texmgr.getRef("playerSprite")) }
{
	// Setup background
	mBackground.setTexture(this->mGame.texmgr.getRef("background"));

	// Let's take care of the views

	sf::Vector2f pos = sf::Vector2f(gViewSize);
	this->mViewBG1.setSize(pos);
	this->mViewBG2.setSize(pos);
	this->mViewGame.setSize(pos);
	this->mViewPlayer.setSize(pos);
	this->mViewHUD.setSize(pos);
	pos *= 0.5f;
	this->mViewBG1.setCenter(pos);
	this->mViewBG2.setCenter(pos);
	this->mViewGame.setCenter(pos);
	this->mViewPlayer.setCenter(pos);
	this->mViewHUD.setCenter(pos);

	// We know how to translate local coords into global ones

	mScrTopLeft = mViewPlayer.getCenter() + mViewOffset;


	/*
	Initialize the player
	*/

	
	mPlayer.setEntity(mPlayerEntity.get());

	mScene.addEntity(std::move(mPlayerEntity));




	/*
	   For temporary test purposes,
	   we shall add in a few entities
	   hard coded until we have a procedural-genrated method
	   that can spawn entities by scripts and functions
	   */

	std::unique_ptr<Entity> q {new Entity(sf::Vector2f(500, 400),
		300,
		this->mGame.texmgr.getRef("metal wall"))};
	mScene.addEntity(std::move(q));
	q.reset(new Entity(sf::Vector2f(600, 400),
		300,
		this->mGame.texmgr.getRef("metal wall")));

	mScene.addEntity(std::move(q));


}

sf::Transform& Level::getViewTransform() const{
	sf::RenderStates tmp;
	return tmp.transform.translate(mScrTopLeft);
}

void Level::render(){

	// First render scene of background

	mWindow.setView(mViewBG1);

	// draw the background
	mWindow.draw(mBackground);

	// Translate the local coordinates to global ones.
	// To do it is to pass around parameters of 
	// where the view is set right now (mView.getCenter + mViewOffset())
	mScene.render(mWindow, getViewTransform());

	


}

void Level::update(const float dt){

	mScene.update(dt);

}