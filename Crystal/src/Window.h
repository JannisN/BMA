#pragma once

#include "OpenGL.h"
#include "Renderer.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Utils.h"

#ifdef WINAPI
#include "Win32Window.h"
#endif

namespace Crystal
{
	class Win32Window;

	class Window
	{
	public:
		Window(bool fullscreen, Renderer* renderer);
		Window(int width, int height, char* title, Renderer* renderer);
		Window(int width, int height, Renderer* renderer);
		Window(Renderer* renderer);
		~Window();

		void setSize(int width, int height, bool changeOld);
		void setSize(int width, int height);
		void setTitle(char* title);

		int getHeight();
		int getWidth();

		void setFullscreen(bool option);
		bool isFullscreen();
		void toggleFullscreen();

		void hideCursor(bool option);

		void setResizable(bool option);

		void enableVSync(bool option);

		void swapBuffers();
		bool windowShouldClose();

		void setRenderer(Renderer* renderer);

		void wrapCursor();
		void setWrap(bool option);

		void updateCursor();
		void update();

		void setCurrent();

	private:
		int width;
		int height;
		int oldWidth;
		int oldHeight;
		char* title;
		bool fullscreen;
		bool vsync;
		Renderer* renderer;

#ifdef WINAPI
		Win32Window* window;
#else
		GLFWwindow* window;
#endif
	};
}