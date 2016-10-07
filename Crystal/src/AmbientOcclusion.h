#pragma once

#include "ScreenEffect.h"

#include <random>

namespace Crystal
{
	class AmbientOcclusion : public ScreenEffect
	{
	public:
		AmbientOcclusion();
		void render(Renderer* renderer, Texture depth, Texture normal, Texture noise);
	private:
		static char* ssaoSource;
		GLuint noiseTexture;
		std::vector<glm::vec3> ssaoKernel;
		std::vector<float> kernel;
	};
}