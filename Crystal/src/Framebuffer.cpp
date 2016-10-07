#include "Framebuffer.h"

namespace Crystal
{
	const GLfloat Framebuffer::vertexBufferData[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};

	Framebuffer::Framebuffer(int width, int height)
	{
		this->width = width;
		this->height = height;

		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		texture.generate();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		Reporter::report(glGetError());

		glGenRenderbuffers(1, &buffer);
		Reporter::report(glGetError());
		glBindRenderbuffer(GL_RENDERBUFFER, buffer);

		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER_WIDTH, GL_RENDERBUFFER_HEIGHT);

		Reporter::report(glGetError());
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_COLOR_COMPONENTS, width, height);
		Reporter::report(glGetError());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, buffer);
		Reporter::report(glGetError());
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer);
		Reporter::report(glGetError());
		/*glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_BUFFER,NULL , 0);
		Reporter::report(glGetError());*/
		
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.id(), 0);



		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Reporter::report("FAIL");

		const char* vertex =
			"#version 400\n"
			"in vec3 aPosition;"
			"out vec2 varTextureCoord;"
			"void main() {"
			"varTextureCoord = vec2((aPosition.x + 1) / 2.0, (aPosition.y + 1) / 2.0);"
			"gl_Position = vec4(aPosition, 1.0);"
			"}";

		const char* fragment =
			"#version 400\n"
			"in vec2 varTextureCoord;"
			"out vec3 color;"
			"uniform sampler2D renderedTexture;"
			"void main() {"
			"color = texture(renderedTexture, varTextureCoord).xyz;"
			"}";
		//texture(renderedTexture, varTextureCoord).xyz * 2;
		shader = Shader(vertex, fragment);
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

		textureLocation = glGetUniformLocation(shader.id(), "renderedTexture");
	}

	void Framebuffer::setSize(int width, int height)
	{
		this->width = width;
		this->height = height;

		glBindRenderbuffer(GL_RENDERBUFFER, buffer);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer);

		/*glBindRenderbuffer(GL_RENDERBUFFER, buffer);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER_WIDTH, GL_RENDERBUFFER_HEIGHT);*/

		texture.bind(0);

		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, width, height, false);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	void Framebuffer::render()
	{
		shader.bind();
		texture.bind(0);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(vao);
		glUniform1i(textureLocation, 0);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexBufferData));
	}

	void Framebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
	}

	void Framebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	Texture Framebuffer::getTexture()
	{
		return texture;
	}
}