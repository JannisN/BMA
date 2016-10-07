#pragma once

#include "Renderer.h"
#include "Timestamp.h"
#include "Vertex.h"

#include <list>


namespace Crystal
{
	class Renderer;

	class Mesh
	{
	public:
		Mesh();
		virtual void update(Timestamp timestamp);
		virtual void render(Renderer* renderer);
		GLuint vao;
		std::list<Vertex> vertices;
		Vec pos;
		Vec size;
		Vec rot;
	};
}