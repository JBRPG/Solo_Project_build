#pragma once

/* This is the parent of all state classes
 *
 *
 */

#include "game.hpp"

class Game;

class State{

	public:

		State( Game*, const std::string&);
		~State();

		virtual void init() = 0;
		virtual void run() = 0;
		virtual void cleanup() = 0;
		virtual void reset() = 0;
		virtual void render() = 0;

		std::string        rName;

	protected:
		void pushRequest(std::string);
		void popRequest();
		void swapRequest(std::string);
		void returnRequest(std::string);
		void clearRequest();

		// References to the engine's resources (window, view, textures, etc.)

		Game*               rGame;
		sf::RenderWindow    rWindow;
		bool                rIsActive;


};