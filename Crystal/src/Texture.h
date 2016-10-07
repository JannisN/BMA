#pragma once

#include "OpenGL.h"
#include "SOIL\soil.h"

#include <string>

namespace Crystal
{
	class Texture
	{
	public:
		Texture();
		Texture(std::string path);
		void load(std::string path);
		void generate();

		GLuint id();

		void bind(int active);
		void unbind(int active);
	private:
		void loadTexture(std::string path);
		GLuint texture;
	};
}