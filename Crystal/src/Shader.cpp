#include "Shader.h"

namespace Crystal
{
	Shader::Shader()
	{

	}

	Shader::Shader(std::string vertex, std::string fragment)
	{
		compileShader(vertex, fragment);
	}

	void Shader::loadFromFile(std::string vertexFile, std::string shaderFile)
	{
		compileShader(loadFile(vertexFile), loadFile(shaderFile));
	}

	void Shader::bind()
	{
		glUseProgram(shader);
	}

	void Shader::unbind()
	{
		glUseProgram(NULL);
	}

	GLint Shader::id()
	{
		return shader;
	}

	void Shader::compileShader(std::string vertex, std::string fragment)
	{
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		const char *vertShaderSrc = vertex.c_str();
		const char *fragShaderSrc = fragment.c_str();

		GLint result = GL_FALSE;
		int logLength;

		glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
		glCompileShader(vertShader);

		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> vertexError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(vertShader, logLength, NULL, &vertexError[0]);

		Reporter::report(&vertexError[0]);//TODO nur reporten wenn auch fehler ist

		glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
		glCompileShader(fragShader);

		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> fragError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(fragShader, logLength, NULL, &fragError[0]);

		Reporter::report(&fragError[0]);

		GLuint program = glCreateProgram();
		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> linkError((logLength > 1) ? logLength : 1);
		glGetProgramInfoLog(program, logLength, NULL, &linkError[0]);

		Reporter::report(&linkError[0]);

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		shader = program;
	}
}