#include "Normalbuffer.h"

namespace Crystal
{
	Normalbuffer::Normalbuffer(int width, int height) : Framebuffer(width, height)
	{
		const std::string vertexShader =
			"#version 400\n"
			"in vec3 aPosition;"
			"in vec3 aNormal;"
			"in vec2 aTextureCoord;"
			"out vec3 varNormal;"
			"uniform mat4 uModelViewProjectionMatrix;"
			"uniform mat4 uProjectionMatrix;"
			"uniform mat4 uNormalScreenMatrix;"
			"uniform mat4 uNormalMatrix;"
			"void main() {"
			"varNormal = (uNormalScreenMatrix * vec4(normalize(aNormal), 1.0)).xyz;"
			"gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);"
			"}";

		const std::string fragmentShader =
			"#version 400\n"
			"layout(location = 0) out vec4 frag_colour;"
			"in vec3 varNormal;"
			"uniform sampler2D texture1;"
			"void main() {"
			"float x = (varNormal.x + 1) / 2.0;"
			"float y = (varNormal.y + 1) / 2.0;"
			"float z = (varNormal.z + 1) / 2.0;"
			"frag_colour = vec4(x, y, z, 1);"
			"}";

		normalShader = new Shader(vertexShader, fragmentShader);
	}

	void Normalbuffer::bind(Renderer* renderer)
	{
		renderer->setShader(normalShader);
		super::bind();
	}
}