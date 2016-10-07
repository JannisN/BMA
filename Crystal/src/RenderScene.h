#pragma once

#include "Renderer.h"
#include "Timestamp.h"
#include "Reporter.h"
#include "Shader.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Utils.h"
#include "Application.h" 
#include "WavefrontModel.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "Depthbuffer.h"
#include "Normalbuffer.h"
#include "AmbientOcclusion.h"
#include "Blur.h"

namespace Crystal
{
	class Application;

	class RenderScene
	{
	public:
		void init(Renderer* renderer);
		void render(Renderer* renderer);
		void update(Timestamp timestamp, Camera* camera);

		//testing
		GLuint vao;
		Mesh* mesh;
		float camera;
		WavefrontModel* cube;
		WavefrontModel* cube1;
		WavefrontModel* sphere;
		Texture* texture;
		Shader* shader2;
		Texture* noise;
	};
}