#include "soundPlayer.hpp"

SoundPlayer::SoundPlayer():
play_sound(new sf::Sound()){

}

SoundPlayer* SoundPlayer::p_instance = 0;

SoundPlayer* SoundPlayer::instance(){
	if (!p_instance){
		p_instance = new SoundPlayer();
	}

	return p_instance;
}

void SoundPlayer::loadSound(std::string const& name, std::string const& filename){

	sounds[name].loadFromFile(filename);
}

sf::SoundBuffer& SoundPlayer::getRef(const std::string& sound){
	
	sf::SoundBuffer& getBuff = sounds.at(sound);
	return getBuff;
}

void SoundPlayer::playSound(const std::string& name){
	play_sound->setBuffer(getRef(name));
	play_sound->play();
}