#pragma once

#include <map>
#include <string>

#include "SFML\Audio.hpp"

class SoundPlayer{
private:
	std::map<std::string, sf::SoundBuffer> sounds;
	sf::Sound* play_sound;


	static SoundPlayer* p_instance;
	SoundPlayer();
	SoundPlayer(SoundPlayer const&);
	SoundPlayer& operator=(SoundPlayer const &);

public:
	static SoundPlayer* instance();

	void loadSound(const std::string&, const std::string&);

	sf::SoundBuffer& getRef(const std::string&);

	void playSound(const std::string&);
};