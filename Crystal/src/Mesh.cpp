#include "Mesh.h"

namespace Crystal
{
	Mesh::Mesh()
	{
		pos = Vec(0, 0, 0);
		size = Vec(1, 1, 1);
		rot = Vec(0, 0, 0);
	}

	void Mesh::update(Timestamp timestamp)
	{
	}

	void Mesh::render(Renderer* renderer)
	{
		renderer->render(this);
	}
}