#include "Vec2D.h"

namespace Crystal
{
	Vec2D::Vec2D()
	{
	}

	Vec2D::Vec2D(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vec2D operator + (Vec2D vector1, Vec2D vector2)
	{
		return Vec2D(vector1.x + vector2.x, vector1.y + vector2.y);
	}

	Vec2D operator - (Vec2D vector1, Vec2D vector2)
	{
		return Vec2D(vector1.x - vector2.x, vector1.y - vector2.y);
	}

	Vec2D operator * (Vec2D vector1, Vec2D vector2)
	{
		return Vec2D(vector1.x * vector2.x, vector1.y * vector2.y);
	}

	Vec2D operator / (Vec2D vector1, Vec2D vector2)
	{
		return Vec2D(vector1.x / vector2.x, vector1.y / vector2.y);
	}
}