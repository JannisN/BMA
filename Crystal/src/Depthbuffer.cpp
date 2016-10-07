#include "Depthbuffer.h"

namespace Crystal
{
	Depthbuffer::Depthbuffer(int width, int height) : Framebuffer(width, height)
	{
		const std::string vertexShader =
			"#version 400\n"
			"in vec3 aPosition;"
			"in vec3 aNormal;"
			"in vec2 aTextureCoord;"
			"out float depth;"
			"uniform mat4 uModelViewProjectionMatrix;"
			"uniform mat4 uNormalMatrix;"
			"uniform mat4 uModelViewMatrix;"
			"void main() {"
			"depth = (uModelViewMatrix * vec4(aPosition, 1.0)).z;"
			"depth = (-depth - 0.01)/(10 - 0.01);"
			"vec4 v = uModelViewProjectionMatrix * vec4(aPosition, 1.0);"
			"gl_Position = v;"
			"}";

		const std::string fragmentShader =
			"#version 400\n"
			"layout(location = 0) out vec4 frag_colour;"
			"in float depth;"
			"uniform sampler2D texture1;"
			"const float NEAR = 0.1;"
			"const float FAR = 10.0f;"
			/*"float LinearizeDepth(float depth)"
			"{"
			"float z = depth * 2.0 - 1.0;"
			"return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));"
			"}"*/
			"void main() {"
			"frag_colour = vec4(depth, depth, depth, 1);"
			"}";

		depthShader = new Shader(vertexShader, fragmentShader);
	}

	void Depthbuffer::bind(Renderer* renderer)
	{
		renderer->setShader(depthShader);
		super::bind();
	}
}