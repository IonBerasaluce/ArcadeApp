#include "Ball.h"
#include "Utils/Utils.h"
#include "Graphics/Screen.h"
#include "Graphics/Colour.h"
#include "Shapes/Circle.h"
#include "BoundaryEdge.h"

const float Ball::RADIUS = 5.0f;

Ball::Ball():Ball(Vec2D::Zero, RADIUS)
{
}

Ball::Ball(const Vec2D& pos, float radius):
	m_BBox(pos - Vec2D(radius, radius), radius*2.0f, radius * 2.0f), m_Velocity(Vec2D::Zero)
{

}

void Ball::Update(uint32_t dt)
{
	m_BBox.MoveBy(m_Velocity * MillisecondsToSeconds(dt));
}

void Ball::Draw(Screen& screen)
{
	Circle circ = { m_BBox.GetCenterPoint(), GetRadius() };
	screen.Draw(circ, Colour::Red(), true, Colour::Red());
}

void Ball::MakeFlushWithEdge(const BoundaryEdge& edge, Vec2D& pointOnEdge, bool limitToEdge)
{
	if (edge.normal == DOWN_DIR)
	{
		m_BBox.MoveTo(Vec2D(m_BBox.GetTopLeft().GetX(), edge.edge.GetP0().GetY() + edge.normal.GetY()));
	}
	else if (edge.normal == UP_DIR)
	{
		m_BBox.MoveTo(Vec2D(m_BBox.GetTopLeft().GetX(), edge.edge.GetP0().GetY() - m_BBox.GetHeight()));
	}
	else if (edge.normal == RIGHT_DIR)
	{
		m_BBox.MoveTo(Vec2D(edge.edge.GetP0().GetX() + edge.normal.GetX(), m_BBox.GetTopLeft().GetY()));
	}
	else if (edge.normal == LEFT_DIR)
	{
		m_BBox.MoveTo(Vec2D(edge.edge.GetP0().GetX() - m_BBox.GetWidth(), m_BBox.GetTopLeft().GetY()));
	}

	pointOnEdge = edge.edge.ClosestPoint(m_BBox.GetCenterPoint(), limitToEdge);
}

void Ball::MoveTo(const Vec2D& point)
{
	m_BBox.MoveTo(point - Vec2D(GetRadius(), GetRadius()));
}

void Ball::Bounce(const BoundaryEdge& edge)
{
	Vec2D pointOnEdge;
	MakeFlushWithEdge(edge, pointOnEdge, false);
	m_Velocity = m_Velocity.Reflect(edge.normal);

}
