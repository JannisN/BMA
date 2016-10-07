#include "Texture.h"

namespace Crystal
{
	Texture::Texture()
	{
	}

	Texture::Texture(std::string path)
	{
		load(path);
	}

	void Texture::load(std::string path)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		loadTexture(path);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Texture::generate()
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	GLuint Crystal::Texture::id()
	{
		return texture;
	}

	void Texture::bind(int active)
	{
		glActiveTexture(GL_TEXTURE0 + active);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void Texture::unbind(int active)
	{
		glActiveTexture(GL_TEXTURE0 + active);
		glBindTexture(GL_TEXTURE_2D, NULL);
	}

	void Texture::loadTexture(std::string path)
	{
		int width, height;
		unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
}