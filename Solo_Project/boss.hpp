#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

#include "enemy.hpp"
#include <vector>
#include <string>


// forward declare
class WeaponTemplate;
class Weapon;
class Movement;
class SceneGame;

/*
    The boss type enemy has unique behaviors:

	phase: will have a list of health values to indicate different weapons
	boss_move: The boss will initally move forward for a small period before switching
	  its velocity to go against the scrolling speed, hence looking like
	  that the craft only moves vertically
	move_bounds: a unique movement reserved for bosses
	  if the boss hits the edge of the screen, its movement speed
	  reverses for a particular direction (up to down, left to right, etc.)
	Weapon list: has equal amount of elements to phases, contains weapon pointers
	boss_name: for constructors, we want to have a seperate functions for producing bosses
	  to reduce the burden of code for the constructor.
	  However, we will only have one boss type for this project

*/


class Boss : public Enemy{


	private:

		// variables
		int phase = 0;
		std::vector<int> boss_phase_hp; // less health at higher indexes
		bool final_phase = false;
		std::vector<Weapon*> weapons;
		std::string boss_name;

		int intro_timer;
		int intro_time_set;


		// functions

		void checkPhase();
		void destroyBoss();
		void death_burst();



	public:

		// inside constructor, make seperate functions to create boss with given name
		Boss(std::string);

		// make a boss with needed data
		Boss(SceneGame*, std::string, int, float, bool,
			sf::Vector2f, Weapon*, Movement*,
			std::string, std::vector<Weapon*>, std::vector<int>, int);

		~Boss();


		// functions



		void collideWith(Entity&);
		void update(float);
		void updateMovement(Movement&);
		void updateWeapon(Weapon&);





};