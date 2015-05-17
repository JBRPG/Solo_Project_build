#include "game.hpp"
#include "scene.hpp"

#include "soundPlayer.hpp"

Game::Game():
multiplier(1)
{

	this->loadTextures();
	this->loadSounds();

	this->window.create(sf::VideoMode (800,600), "Solo Project Ver 0.0");
	this->window.setFramerateLimit(60);
}

Game::~Game(){

	// Will pop remaining states

}

void Game::gameLoop(){
	sf::Clock clock;

	while (this->window.isOpen()){
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();

		if (peekScene() == nullptr) continue;
		// Update the user input
		// update whatever entity is in active scene
		peekScene()->update(dt);
		this->window.clear(sf::Color::Black);
		// draw the entities within the active scene
		peekScene()->draw(dt);
		peekScene()->handleInput();

		this->window.display();

	}

}

void Game::pushScene(Scene* scene){
	this->scenes.push(scene);
}

void Game::popScene(){
	delete this->scenes.top();
	this->scenes.pop();
}

void Game::changeScene(Scene* scene){
	if (!this->scenes.empty()){
		popScene();
	}
	pushScene(scene);
}

Scene* Game::peekScene(){
	if (this->scenes.empty()) return nullptr;
	return this->scenes.top();
}



void Game::loadTextures(){

	std::string current_line;

	std::ifstream target_file("scripts/texture_list.txt");

	/*
	texture_list will have two tokens per line,
	string name (0), and filename (1), seprated by ", "
	*/

	if (target_file.is_open()){

		while (std::getline(target_file, current_line)){
			std::vector<std::string> elems = Game::split(current_line, ", ");
			TextureManager::instance()->loadTexture(elems.at(0), elems.at(1));
		}
		target_file.close();
	}

	else{
		std::cout << "Cannot open file. Close program";
		this->window.close();
	}
}

void Game::loadSounds(){

	std::string current_line;

	std::ifstream target_file("scripts/sound_list.txt");

	/*
	texture_list will have two tokens per line,
	string name (0), and filename (1), seprated by ", "
	*/

	if (target_file.is_open()){

		while (std::getline(target_file, current_line)){
			std::vector<std::string> elems = Game::split(current_line, ", ");
			SoundPlayer::instance()->loadSound(elems.at(0), elems.at(1));
		}
		target_file.close();
	}

	else{
		std::cout << "Cannot open file. Close program";
		this->window.close();
	}
}


std::vector<std::string> Game::split(const std::string &line, std::string delim,
	std::vector<std::string> &elems){

	unsigned int start = 0U;
	unsigned int end = line.find(delim);

	while (end != std::string::npos){
		elems.push_back(line.substr(start, end - start));
		start = end + delim.length();
		end = line.find(delim, start);
	}
	elems.push_back(line.substr(start, end));

	return elems;
}

std::vector<std::string> Game::split(const std::string &line, std::string delim){
	std::vector<std::string> elems;
	Game::split(line, delim, elems);
	return elems;
}

int Game::getScore(){
	return score;
}

void Game::setScore(int _score){
	score = _score;
}

int Game::getHiScore(){
	return highscore;
}

void Game::setHiScore(){
	highscore = highscore < score ? score : highscore;
}

void Game::increaseScore(int bonus){
	score += bonus * multiplier;
}

int Game::getMultiplier(){
	return multiplier;
}

void Game::setMultiplier(int _mult){
	multiplier = _mult;
}

void Game::increaseMultiplier(){
	++multiplier;
}

