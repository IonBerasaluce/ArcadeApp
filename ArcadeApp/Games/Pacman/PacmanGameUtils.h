#pragma once

#include "Math/Vec2D.h"
#include <vector>


static const int PACMAN_MOVEMENT_SPEED = 50;
static const int GHOST_MOVEMENT_SPEED = 50;
static const int GHOST_VULNERABLE_MOVEMENT_SPEED = 25;
static const int GHOST_BACK_TO_PEN_SPEED = 120;

enum PacmanMovement
{
	PACMAN_MOVE_NONE = 0,
	PACMAN_MOVE_UP,
	PACMAN_MOVE_DOWN,
	PACMAN_MOVE_LEFT,
	PACMAN_MOVE_RIGHT
};

Vec2D GetMovementVector(PacmanMovement direction);
PacmanMovement GetOppositeDirection(PacmanMovement direction);
std::vector<PacmanMovement> GetPerpendicularMovements(PacmanMovement direction);