#include "textureManager.hpp"


TextureManager* TextureManager::p_instance = 0;

TextureManager* TextureManager::instance(){
	if (!p_instance)
		p_instance = new TextureManager;
	return p_instance;
}
void TextureManager::loadTexture(const std::string& name, const std::string& filename){

	// Load texture
	sf::Texture tex;
	tex.loadFromFile(filename);

	// add to list of textures
	this->textures[name] = tex;
}

sf::Texture& TextureManager::getRef(const std::string& texture){
	return this->textures.at(texture);
}