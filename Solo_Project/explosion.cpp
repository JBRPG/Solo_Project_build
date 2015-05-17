#include "explosion.hpp"


Explosion::Explosion(const sf::Texture& tex, const sf::IntRect& rect, int delay) :
Entity(tex, rect, 0, 0, true), frame_wait_set(delay){
	tex_frames = sf::Vector2i(tex.getSize().x / rect.width, tex.getSize().x / rect.width);
	frames = tex_frames.x * tex_frames.y;
}

 void Explosion::update(float dt){
	 ++ticks;
	 spawnTime -= dt;
	 changeFrame();
	 checkExplosion();
}

 void Explosion::changeFrame(){
	 if (frames == 0) return;

	 if (ticks % frame_wait_set == 0){
		 curr_frame++;
		 int curr_framerectX = curr_frame % tex_frames.x;
		 int curr_framerectY = curr_frame / tex_frames.x;
		 
		 // do not change frame if it goes over frame limit
		 if (curr_frame >= frames) return;
		 this->setTextureRect(sf::IntRect(curr_framerectX * int(this->getTextureRect().width),
			                  curr_framerectY * int(this->getTextureRect().height),
							  getTextureRect().width, getTextureRect().height));
	 }
 }

 void Explosion::checkExplosion() {
	 if ((frames == 0 && spawnTime <= 0) || (curr_frame >= frames))
		 myScene->storeRemovedEntity(this);
 };