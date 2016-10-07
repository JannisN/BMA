#pragma once

#include "Vec.h"

namespace Crystal
{
	class Vertex
	{
	public:
		Vertex();
		Vertex(Vec pos);
		Vertex(Vec pos, Vec normal);
		Vertex(Vec pos, Vec normal, Vec uvCoord);
		Vec pos;
		Vec uvCoord;
		Vec normal;
		Vec color;
	};
}