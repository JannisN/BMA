#pragma once

#include "Utils.h"
#include "OpenGL.h"
#include "Reporter.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

namespace Crystal
{
	class Shader
	{
	public:
		Shader();
		Shader(std::string vertex, std::string fragment);

		void loadFromFile(std::string vertexFile, std::string shaderFile);
		
		void bind();
		void unbind();

		GLint id();
	
	private:
		void compileShader(std::string vertex, std::string fragment);
		GLint shader;
	};
}