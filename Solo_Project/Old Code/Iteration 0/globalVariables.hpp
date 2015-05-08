/* Multiple classes will use these variables for the game*/

// Enum is a common choice for modular coding
// however, you have to use them appropriately

#pragma once

#include <SFML\Graphics.hpp>

enum class CommandType { ACCELERATE, BACKGROUND, DEATH_ENEMY, DEATH_PLAYER, 
	                     DONOTHING, EXPLODE_SPAWN, FIRE_ENEMY, FIRE_PLAYER,
						 MOVE, ROTATE, SLOWDOWN, SPAWN_ENEMY, SPAWN_PLAYER,
						 STAGECLEAR, STAGEOVER, TRACK, TURN};

enum class ControlType { KEYBOARD, GAMEPAD };

enum class EntityType {	PLAYER, ENEMY, BULLET, PLAYERBULLET, ENEMYBULLET,
                        TERRAIN, BACKGROUND};

enum class SAID {PUSH, POP, SWAP, RETURN_TO, CLEAR, NOTHING}; // Stack Action ID

// Non enum section

const std::string gVersion {"alpha 0.0"};

const sf::Vector2u   gViewSize {800, 640};
const double         gDesFPS { 60.0 }; // Desired FPS
const sf::Time       gTimePerFrame{ sf::seconds(1.0 / gDesFPS) };
const int            gMaxEnemyCount { 100 };