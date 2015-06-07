#include "particle_animated.hpp"

AnimatedParticle::AnimatedParticle(const sf::Texture& tex, const sf::IntRect& rect, int delay) :
Entity(tex, rect, 0, 0, true), frame_wait_set(delay){
	tex_frames = sf::Vector2i(tex.getSize().x / rect.width, tex.getSize().y / rect.height);
	frames = tex_frames.x * tex_frames.y;
}

void AnimatedParticle::update(float dt){
	++ticks;
	changeFrame();
}


// unlike explosion, we make the animation cycle through endlessly
void AnimatedParticle::changeFrame(){
	if (frames == 0) return;

	if (ticks % frame_wait_set == 0){
		curr_frame++;
		if (curr_frame >= frames) curr_frame = 0;
		int curr_framerectX = curr_frame % tex_frames.x;
		int curr_framerectY = curr_frame / tex_frames.x;

		// do not change frame if it goes over frame limit
		if (curr_frame >= frames) return;
		this->setTextureRect(sf::IntRect(curr_framerectX * int(this->getTextureRect().width),
			curr_framerectY * int(this->getTextureRect().height),
			getTextureRect().width, getTextureRect().height));
	}
}
