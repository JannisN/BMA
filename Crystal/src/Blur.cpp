#include "Blur.h"

namespace Crystal
{
	char* Blur::blurSource =
		"#version 330 core\n"
		"out vec4 FragColor;"
		"in vec2 varTextureCoord;"
		"uniform sampler2D uTexInput;"
		"uniform sampler2D uTexInput2;"

		"uniform int uBlurSize = 1;"

		"out vec4 fResult;"

		"void main() {"
		"vec2 texelSize = 1.0 / vec2(textureSize(uTexInput, 0));"
		"float result = 0.0;"
		"for (int x = -2; x < 2; ++x)"
		"{"
		"	for (int y = -2; y < 2; ++y)"
		"	{"
		"		vec2 offset = vec2(float(x), float(y))/* * texelSize*/;"
		"		vec2 screen = vec2(textureSize(uTexInput, 0));"
		"		result += texture(uTexInput, varTextureCoord + offset / screen).r;"
		"	}"
		"}"
		"float r = result / (4.0 * 4.0);"
		//"fResult = texture(uTexInput, varTextureCoord);"
		"fResult = vec4(r) * texture(uTexInput2, varTextureCoord);"
		/*"vec2 texelSize = 1.0 / vec2(textureSize(uTexInput, 0));"
		"float result = 0.0;"
		"vec2 hlim = vec2(float(-uBlurSize) * 0.5 + 0.5);"
		"for (int i = 0; i < uBlurSize; ++i) {"
		"	for (int j = 0; j < uBlurSize; ++j) {"
		"		vec2 offset = (hlim + vec2(float(i), float(j))) * texelSize;"
		"		result += texture(uTexInput, varTextureCoord + offset).r;"
		"	}"
		"}"
		
		"fResult = vec4(result / float(uBlurSize * uBlurSize));"*/
		"}";

	Blur::Blur() : ScreenEffect(blurSource)
	{
	}

	void Blur::render(Renderer * renderer, Texture texture, Texture texture2)
	{
		shader.bind();
		texture.bind(0);
		texture2.bind(1);

		glBindVertexArray(vao);

		GLint textureLocation = glGetUniformLocation(shader.id(), "uTexInput");
		glUniform1i(textureLocation, 0);

		textureLocation = glGetUniformLocation(shader.id(), "uTexInput2");
		glUniform1i(textureLocation, 1);

		glDrawArrays(GL_TRIANGLES, 0, 18 * sizeof(float));
	}
}