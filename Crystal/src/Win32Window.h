#pragma once

#include "OpenGL.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Application.h"
#include "Reporter.h"

#include <Windows.h>
#include <thread>

namespace Crystal
{
	class Win32Window
	{
	public:
		Win32Window(int width, int height, bool resizable, char* title, HINSTANCE hInstance);
		~Win32Window();

		void setFullscreen(bool fullscreen);
		void setFullscreen(bool fullscreen, int width, int height);
		void toggleFullscreen();
		void toggleFullscreen(int width, int height);

		int getWidth();
		int getHeight();

		void updateWindow();
		void SwapBuffer();
		void changeCursor(bool visibility);
		void changeWrap(bool wrap);

		void setWindowSize(int width, int height);
		bool shouldClose();

		void updateLoop();

		HDC m_hDeviceContext;

	private:
		bool close;
		int realWidth, realHeight;
		int width, height, icon, posX, posY, secWidth, secHeight;
		bool resizable, fullscreen, cursorVisibility, wrapCursor, maximized;
		char* title;
		RECT pos;

		void RegisterWindowClass();
		void CreateContext();
		RECT Win32Window::GetLocalCoordinates() const;

		void OnSize(GLsizei width, GLsizei height);

		static LRESULT CALLBACK OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
		void ProcessEvent(UINT Message, WPARAM wParam, LPARAM lParam);

		HWND m_hWindow;
		HGLRC m_hGLContext;
		HINSTANCE m_hInstance;
	};
}