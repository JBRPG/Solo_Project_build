#include "input.hpp"

Input* Input::p_instance = 0;

Input* Input::instance(){
	if (!p_instance)
		p_instance = new Input;
	return p_instance;
}

bool Input::pressKeybutton(sf::Keyboard::Key key){

	return sf::Keyboard::isKeyPressed(key);
}