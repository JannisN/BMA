#pragma once

#include "Window.h"
#include "Renderer.h"
#include "Timestamp.h"
#include "Reporter.h"
#include "RenderScene.h"

#include <thread>

namespace Crystal
{
	class Window;
	class RenderScene;
	class Mouse;

	class Application
	{
	public:
		Application();
		~Application();

		static Application* getActiveApplication();

		void run();
		void quit();

		void createWindow();
		void createWindow(int width, int height);
		void createWindow(char* title);
		void createWindow(bool fullscreen);
		void createWindow(int width, int height, char* title);
		void createWindow(int width, int height, char* title, bool fullscreen);

		void setRendererResolution(int width, int height);

		Window* getWindow();

		void onRender();

	private:
		static void setActiveApplication(Application* app);
		static Application* application;

		
		Window* window;
		bool running;
		Renderer* renderer;
		RenderScene* renderScene;
		Timestamp timestamp;
	};
}