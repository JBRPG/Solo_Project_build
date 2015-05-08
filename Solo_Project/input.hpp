#pragma once

#include "SFML\Graphics.hpp"

//We will make this class a Singleton

class Input{

private:

	Input() {}; // this constructor cannot be called since it remains private.
	Input(Input const&) {}; // copy constructor is private
	Input& operator=(Input const&) {}; // assingment is private
	static Input* p_instance;

public:

	static Input* instance();
	bool pressKeybutton(sf::Keyboard::Key key);


};