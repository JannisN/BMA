#include "Vertex.h"

namespace Crystal
{
	Vertex::Vertex()
	{
	}

	Vertex::Vertex(Vec pos)
	{
		this->pos = pos;
	}

	Vertex::Vertex(Vec pos, Vec normal)
	{
		this->pos = pos;
		this->normal = normal;
	}

	Vertex::Vertex(Vec pos, Vec normal, Vec uvCoord)
	{
		this->pos = pos;
		this->normal = normal;
		this->uvCoord = uvCoord;
	}
}