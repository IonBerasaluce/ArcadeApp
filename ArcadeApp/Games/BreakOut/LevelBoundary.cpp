/*
 * LevelBoundary.cpp
 *
 *  Created on: Jan. 16, 2019
 *      Author: serge
 */

#include "LevelBoundary.h"
#include "Ball.h"

LevelBoundary::LevelBoundary(const AARectangle& boundary)
{
	m_Includer.Init(boundary, true);
}

bool LevelBoundary::HasCollided(const Ball& ball, BoundaryEdge& edge)
{
	for(int i = 0; i < NUM_EDGES; ++i)
	{
		BoundaryEdge collisionEdge = m_Includer.GetEdge(static_cast<EdgeType>(i));

		if(HasCollidedWithEdge(ball, collisionEdge))
		{
			edge = collisionEdge;
			return true;
		}
	}

	return false;
}

bool LevelBoundary::HasCollidedWithEdge(const Ball& ball, const BoundaryEdge& edge) const
{
	return edge.edge.MinDistanceFrom(ball.GetPosition()) < ball.GetRadius();
}
