#include "PacmanGameUtils.h"
#include "Pacman.h"

Vec2D GetMovementVector(PacmanMovement direction)
{
	if (direction == PacmanMovement::PACMAN_MOVE_LEFT)
	{
		return Vec2D(-1, 0);
	}
	else if (direction == PacmanMovement::PACMAN_MOVE_DOWN)
	{
		return Vec2D(0, 1);
	}
	else if (direction == PacmanMovement::PACMAN_MOVE_RIGHT)
	{
		return Vec2D(1, 0);
	}
	else if (direction == PacmanMovement::PACMAN_MOVE_UP)
	{
		return Vec2D(0, -1);
	}

	return Vec2D::Zero;
}

PacmanMovement GetOppositeDirection(PacmanMovement direction)
{
	if (direction == PacmanMovement::PACMAN_MOVE_LEFT)
	{
		return PACMAN_MOVE_RIGHT;
	}
	else if (direction == PacmanMovement::PACMAN_MOVE_DOWN)
	{
		return PACMAN_MOVE_UP;
	}
	else if (direction == PacmanMovement::PACMAN_MOVE_RIGHT)
	{
		return PACMAN_MOVE_LEFT;
	}
	else if (direction == PacmanMovement::PACMAN_MOVE_UP)
	{
		return PACMAN_MOVE_DOWN;
	}

	return PACMAN_MOVE_NONE;
}

std::vector<PacmanMovement> GetPerpendicularMovements(PacmanMovement direction)
{
	std::vector<PacmanMovement> perpendicularDirections;

	if (direction == PACMAN_MOVE_DOWN || direction == PACMAN_MOVE_UP)
	{
		perpendicularDirections.push_back(PACMAN_MOVE_LEFT);
		perpendicularDirections.push_back(PACMAN_MOVE_RIGHT);
	}
	else if (direction == PACMAN_MOVE_RIGHT || direction == PACMAN_MOVE_LEFT)
	{
		perpendicularDirections.push_back(PACMAN_MOVE_UP);
		perpendicularDirections.push_back(PACMAN_MOVE_DOWN);
	}
	else if (direction == PACMAN_MOVE_NONE)
	{
		perpendicularDirections.push_back(PACMAN_MOVE_LEFT);
		perpendicularDirections.push_back(PACMAN_MOVE_RIGHT);
		perpendicularDirections.push_back(PACMAN_MOVE_UP);
		perpendicularDirections.push_back(PACMAN_MOVE_DOWN);
	}

	return perpendicularDirections;
}
