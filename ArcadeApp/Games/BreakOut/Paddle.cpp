#include "Paddle.h"
#include "Ball.h"
#include "Graphics/Screen.h"
#include "Utils/Utils.h"
#include "Math/Vec2D.h"
#include <cassert>

#include <iostream>

void Paddle::Init(const AARectangle& rect, const AARectangle& boundary)
{
	Excluder::Init(rect);
	m_Boundary = boundary;
	m_Direction = 0;
}

void Paddle::Update(uint32_t dt, Ball& ball)
{
	if (GetAARectangle().ContainsPoint(ball.GetPosition()))
	{
		Vec2D pointOnEdge;
		ball.MakeFlushWithEdge(GetEdge(BOTTOM_EDGE), pointOnEdge, true);
	}

	if (m_Direction != 0)
	{
		Vec2D dir;
		if ((m_Direction & PaddleDirection::LEFT) == PaddleDirection::LEFT && (m_Direction & PaddleDirection::RIGHT) == PaddleDirection::RIGHT)
		{
			dir = Vec2D::Zero;
		}
		else if (m_Direction == PaddleDirection::LEFT)
		{
			dir = LEFT_DIR;
		}
		else
		{
			dir = RIGHT_DIR;
		}

		Vec2D dx = dir * VELOCITY * MillisecondsToSeconds(dt);

		MoveBy(dx);

		const AARectangle& aaRect = GetAARectangle();

		if (IsGreaterThanOrEqual(m_Boundary.GetTopLeft().GetX(), aaRect.GetTopLeft().GetX()))
		{
			MoveTo(Vec2D(m_Boundary.GetTopLeft().GetX(), aaRect.GetTopLeft().GetY()));
		}
 
		if (IsGreaterThanOrEqual(aaRect.GetBottomRight().GetX(), m_Boundary.GetBottomRight().GetX()))
		{
			MoveTo(Vec2D(m_Boundary.GetBottomRight().GetX() - aaRect.GetWidth(), aaRect.GetTopLeft().GetY()));
		}
	}
}

void Paddle::Draw(Screen& screen)
{
	screen.Draw(GetAARectangle(), Colour::Blue(), true, Colour::Blue());
}

bool Paddle::Bounce(Ball& ball)
{
	BoundaryEdge edge;
	if (HasCollided(ball.GetBoundingRect(), edge))
	{
		Vec2D pointOnEdge;
		ball.MakeFlushWithEdge(edge, pointOnEdge, false);

		if (edge.edge == GetEdge(TOP_EDGE).edge)
		{
			float edgeLength = edge.edge.Length();
			assert(!IsEqual(edgeLength, 0));
			float tx = (pointOnEdge.GetX() - edge.edge.GetP0().GetX()) / edgeLength;

			if (((tx <= CORNER_BOUNCE_AMT) && ball.GetVelocity().GetX() > 0) ||
				(tx >= (1.0f - CORNER_BOUNCE_AMT) && ball.GetVelocity().GetX() < 0))
			{
				ball.SetVelocity(-ball.GetVelocity());
				return true;
			}
		}
		ball.SetVelocity(ball.GetVelocity().Reflect(edge.normal));
		return true;
	}
	return false;
}
