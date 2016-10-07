#pragma once

#include "ScreenEffect.h"

namespace Crystal
{
	const GLfloat ScreenEffect::vertexBufferData[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	ScreenEffect::ScreenEffect(char* fragmentSource)
	{
		const char* vertexSource =
			"#version 450\n"
			"in vec3 aPosition;"
			"out vec2 varTextureCoord;"
			"void main() {"
			"varTextureCoord = vec2((aPosition.x + 1) / 2.0, (aPosition.y + 1) / 2.0);"
			"gl_Position = vec4(aPosition, 1.0);"
			"}";

		shader = Shader(vertexSource, fragmentSource);
		shader.bind();

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

		GLint location = glGetAttribLocation(shader.id(), "aPosition");
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//location = glGetUniformLocation(shader.id(), "aProjectionMatrix");
		//glUniformMatrix4fv(location, 1, GL_FALSE, (renderer->getProjectionMatrix()).get());
	}

	void ScreenEffect::render()
	{
		shader.bind();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexBufferData));
	}
}