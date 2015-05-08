#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <iostream>
#include <memory>
#include "globalVariables.hpp"
#include "state.hpp"

class State;

class Game{



	public:

		// Constructors
		Game();
		~Game();

		// Functions
		void run();
		void request();

		// Accessible variables though game-> reference

		sf::RenderWindow      gameWindow;
		sf::View              gameView;

   	private:
		template <typename T>  // Creates appropriate State
		void launchState();    // and pushes it to state stack

		void executeStackChanges(); // the dispatcher function that executes push, pop requests
		void pushState(const std::string&);
		void popState();
		void swapState(const std::string&);
		void clearStateStack();
		void returnToState(const std::string&); // pop as many as needed
		                                        // to reach designated state

		// states and stack-related
		std::vector < std::unique_ptr<State> > rStateStack;

		std::string                            rStrParam;
		sf::Event                              rEvtParam;


		SAID                                   rStackAction;
		std::string                            rStackParam;


};