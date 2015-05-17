#include <SFML\Graphics.hpp>
#include <vector>

#include "animationHandler.hpp"

void AnimationHandler::update(const float dt){
	if (currentAnim >= this->animations.size() || currentAnim < 0) return;

	float duration = this->animations[currentAnim].duration;

	/* Check if animation has progressed to a new frame
	   and if so change to next frame*/
	if (int(t + dt) / duration > int(t / duration)){
		// Calculate the frame number
		int frame = int((t + dt) / duration);

		// Adjust for looping
		frame %= this->animations[currentAnim].getLength();

		/* Set the sprite to the new frame*/
		sf::IntRect rect = this->frameSize;
		rect.left = rect.width * frame;
		rect.top = rect.height * this->currentAnim;
		this->bounds = rect;
	}

	/* increment the time elapsed */
	this->t += dt;
	/* adjust for looping*/
	if (this->t > duration * this->animations[currentAnim].getLength()){
		this->t = 0.0f;
	}

}

void AnimationHandler::addAnim(Animation& anim){
	this->animations.push_back(anim);
}

void AnimationHandler::changeAnim(unsigned int animID){
	/* Do not change the animation
	if the animation is currently active
	or the new animation does not exist */
	if (this->currentAnim == animID ||
		animID >= this->animations.size() || animID < 0) return;

	/* Set the current animation */
	this->currentAnim = animID;
	/* Update the animation bounds */
	sf::IntRect rect = this->frameSize;
	rect.top = rect.height * animID;
	this->bounds = rect;
	this->t = 0.0f;
}