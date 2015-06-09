#include "particle_animated.hpp"

AnimatedParticle::AnimatedParticle(const sf::Texture& tex, const sf::IntRect& rect, int delay) :
Entity(tex, rect, 0, 0, true), frame_wait_set(delay), frame_wait(delay){
	tex_frames = sf::Vector2i(tex.getSize().x / rect.width, tex.getSize().y / rect.height);
	frames = tex_frames.x * tex_frames.y;
}

void AnimatedParticle::update(float dt){
	frame_wait--;
	changeFrame();
}


// unlike explosion, we make the animation cycle through endlessly
void AnimatedParticle::changeFrame(){
	if (frames == 0) return;

	if (frame_wait == 0){
		frame_wait = frame_wait_set;
		curr_frame++;
		if (curr_frame >= frames) curr_frame = 0;
		int curr_framerectX = curr_frame % tex_frames.x;
		int curr_framerectY = curr_frame / tex_frames.x;

		this->setTextureRect(sf::IntRect(curr_framerectX * int(this->getTextureRect().width),
			curr_framerectY * int(this->getTextureRect().height),
			getTextureRect().width, getTextureRect().height));
	}
}

void AnimatedParticle::resetAnimation(){
	frame_wait = frame_wait_set;
	curr_frame = 0;
	
	this->setTextureRect(sf::IntRect(0,0,
		getTextureRect().width, getTextureRect().height));

}
