/*
  These variables and enums will be commonly used by many files
*/

#pragma once

// Enums

enum class EntityType { TERRAIN, ENEMY, PLAYER, BULLET_ENEMY, BULLET_PLAYER, BACKGROUND };

enum class StatusID {ONSCREEN, ONBUFFER, EXPLODING, DESTROYED, SPAWNING};

// Variables

const sf::Vector2u gViewSize { 800, 600 }; // size of actual playing field
const int gMaxEntityCount { 100 };


const sf::Vector2u gGridSize {20, 20}; // size of game grid
const size_t       gGridPointCount{
	                 (gGridSize.x + gGridSize.y) * 2 };
