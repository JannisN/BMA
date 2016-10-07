#pragma once

#include "OpenGL.h"
#include "Texture.h"
#include "Shader.h"
#include "Renderer.h"

namespace Crystal
{
	class ScreenEffect
	{
	public:
		ScreenEffect(char* fragmentSource);
		virtual void render();

	protected:
		Shader shader;
		GLuint vao;

		static const GLfloat vertexBufferData[];
	};
}