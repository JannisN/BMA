#pragma once

#include "Framebuffer.h"
#include "Renderer.h"

namespace Crystal
{
	class Depthbuffer : public Framebuffer
	{
	public:
		Depthbuffer(int width, int height);
		void bind(Renderer* renderer);
	private:
		typedef Framebuffer super;
		Shader* depthShader;
	};
}