#pragma once

namespace Crystal
{
	class Vec2D
	{
	public:
		Vec2D();
		Vec2D(float x, float y);
		float x, y;
	};

	Vec2D operator + (Vec2D vector1, Vec2D vector2);
	Vec2D operator - (Vec2D vector1, Vec2D vector2);
	Vec2D operator * (Vec2D vector1, Vec2D vector2);
	Vec2D operator / (Vec2D vector1, Vec2D vector2);
}