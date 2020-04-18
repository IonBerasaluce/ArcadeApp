#pragma once

#include "Shapes/AARectangle.h"
#include "Math/Vec2D.h"

class Screen;
struct BoundaryEdge;

class Ball
// Internally represented as a square but will be representad as a circle
{
public:
	Ball();
	Ball(const Vec2D& pos, float radius);
	void Update(uint32_t dt);
	void Draw(Screen& screen);
	void MakeFlushWithEdge(const BoundaryEdge& edge, Vec2D& pointOnEdge, bool limitToEdge);

	inline void Stop() { m_Velocity = Vec2D::Zero; }
	void MoveTo(const Vec2D& point);
	inline const AARectangle GetBoundingRect() const { return m_BBox; }

	void Bounce(const BoundaryEdge& edge);

	inline void SetVelocity(const Vec2D& vel) { m_Velocity = vel; }
	inline Vec2D GetVelocity() const { return m_Velocity; }
	inline float GetRadius() const { return m_BBox.GetWidth() / 2.0f; }
	inline Vec2D GetPosition() const { return m_BBox.GetCenterPoint(); }



private:
	AARectangle m_BBox;
	Vec2D m_Velocity;
	static const float RADIUS;
};