#pragma once

#include "OpenGL.h"
#include "Texture.h"
#include "Shader.h"

namespace Crystal
{
	class Framebuffer
	{
	public:
		Framebuffer(int width, int height);
		void setSize(int width, int height);

		virtual void render();

		void bind();
		void unbind();

		Texture getTexture();

	private:
		int width, height;
		GLuint framebuffer;
		Texture texture;
		GLuint vao;
		Shader shader;
		GLuint textureLocation;
		GLuint buffer;

		static const GLfloat vertexBufferData[];
	};
}