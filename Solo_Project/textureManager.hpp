#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class TextureManager{

private:
	// Texture array used
	std::map<std::string, sf::Texture> textures;

	// Constructor
	TextureManager(){};
	TextureManager(TextureManager const&) {}; // copy constructor is private
	TextureManager& operator=(TextureManager const&) {}; // assingment is private
	static TextureManager* p_instance;

public:

	static TextureManager* instance();

	// Add texture from file
	void loadTexture(const std::string& name, const std::string& filename);

	// Translate the id into reference
	sf::Texture& getRef(const std::string& texture);

	

};