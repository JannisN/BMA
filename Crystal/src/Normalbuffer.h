#pragma once

#include "Framebuffer.h"
#include "Renderer.h"

namespace Crystal
{
	class Normalbuffer : public Framebuffer
	{
	public:
		Normalbuffer(int width, int height);
		void bind(Renderer* renderer);
	private:
		typedef Framebuffer super;
		Shader* normalShader;
	};
}