#include "Win32Window.h"

namespace Crystal
{
	Win32Window::Win32Window(int width, int height, bool resizable, char* title, HINSTANCE hInstance) : m_hWindow(NULL), m_hDeviceContext(NULL), m_hGLContext(NULL), m_hInstance(hInstance)
	{
		realWidth = width;
		realHeight = height;

		close = false;

		this->width = width;
		this->height = height;
		this->resizable = resizable;
		this->title = title;
		this->cursorVisibility = true;
		this->icon = icon;
		this->maximized = false;

		RegisterWindowClass();

		RECT WindowRect;
		WindowRect.top = WindowRect.left = 0;
		WindowRect.right = width;
		WindowRect.bottom = height;

		DWORD dwExStyle = 0;
		DWORD dwStyle = 0;

		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

		if (resizable)
			dwStyle = WS_OVERLAPPEDWINDOW;
		else
			dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);
		m_hWindow = CreateWindowEx(dwExStyle, (LPCSTR)title,//hier muss auch evt bearbeitet werden
			(LPCSTR)this->title,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
			0, 0, WindowRect.right - WindowRect.left,
			WindowRect.bottom - WindowRect.top,
			NULL, NULL,
			GetModuleHandle(NULL),
			this);

		MoveWindow(m_hWindow, (GetSystemMetrics(SM_CXSCREEN) - width) / 2, (GetSystemMetrics(SM_CYSCREEN) - height) / 2, width, height, TRUE);

		CreateContext();
		ShowWindow(m_hWindow, SW_SHOW);
		OnSize(width, height);
		glewInit();
	}

	Win32Window::~Win32Window()
	{
		if (this->fullscreen)
		{
			ChangeDisplaySettings(NULL, 0);
			ShowCursor(TRUE);
		}

		if (m_hGLContext)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_hGLContext);
			m_hGLContext = NULL;
		}
		if (m_hDeviceContext)
		{
			ReleaseDC(m_hWindow, m_hDeviceContext);
			m_hDeviceContext = NULL;
		}

		DestroyWindow(m_hWindow);
		UnregisterClass((LPCSTR)this->title, GetModuleHandle(NULL));//muss evt verbessert werden, siehe original
	}

	void Win32Window::setWindowSize(int width, int height)
	{
		if (width != 0)
			this->width = width;
		else
			this->width = GetSystemMetrics(SM_CXSCREEN);
		if (height != 0)
			this->height = height;
		else
			this->height = GetSystemMetrics(SM_CYSCREEN);

		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		if (width != 0)
			dmScreenSettings.dmPelsWidth = this->width;
		else
			dmScreenSettings.dmPelsWidth = GetSystemMetrics(SM_CXSCREEN);
		if (height != 0)
			dmScreenSettings.dmPelsHeight = this->height;
		else
			dmScreenSettings.dmPelsWidth = GetSystemMetrics(SM_CYSCREEN);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		ChangeDisplaySettings(&dmScreenSettings, NULL);
	}

	bool Win32Window::shouldClose()
	{
		return close;
	}

	void threadUpdate()
	{
		MSG Message;
		Message.message = ~WM_QUIT;

		while (Message.message != WM_QUIT)
		{
			while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
		}
	}

	void Win32Window::updateLoop()
	{
		/*std::thread thread(threadUpdate);
		thread.join();*/

		MSG Message;
		Message.message = ~WM_QUIT;

		while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	void Win32Window::toggleFullscreen()
	{
		if (fullscreen)
			setFullscreen(false);
		else
			setFullscreen(true);
	}

	void Win32Window::toggleFullscreen(int width, int height)
	{
		if (fullscreen)
			setFullscreen(false, width, height);
		else
			setFullscreen(true, width, height);
	}

	void Win32Window::setFullscreen(bool fullscreen, int width, int height)
	{
		if (width == NULL)
			width = GetSystemMetrics(SM_CXSCREEN);

		if (height == NULL)
			height = GetSystemMetrics(SM_CYSCREEN);

		this->fullscreen = fullscreen;

		if (fullscreen)
		{
			GetWindowRect(m_hWindow, &pos);

			posX = pos.left;
			posY = pos.top;

			DEVMODE dmScreenSettings;
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = width;
			dmScreenSettings.dmPelsHeight = height;
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

			SetWindowLongPtr(m_hWindow, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
			MoveWindow(m_hWindow, 0, 0, width, height, TRUE);

			glViewport(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		}
		else
		{
			DEVMODE dmScreenSettings;
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			//dmScreenSettings.dmPelsWidth = this->width;
			//dmScreenSettings.dmPelsHeight = this->height;
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

			ChangeDisplaySettings(&dmScreenSettings, NULL);

			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = this->width;
			rect.bottom = this->height;
			if (maximized && this->width == GetSystemMetrics(SM_CXSCREEN))//unschöne lösung
			{
				SetWindowLongPtr(m_hWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE);
				AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE, FALSE);
			}
			else
			{
				SetWindowLongPtr(m_hWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
				AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
			}
			MoveWindow(m_hWindow, posX, posY, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		}
		//dwExStyle = WS_EX_APPWINDOW;
		//dwStyle = WS_POPUP;
		//ShowCursor(FALSE);

	}

	void Win32Window::setFullscreen(bool fullscreen)
	{
		this->fullscreen = fullscreen;

		if (fullscreen)
		{
			GetWindowRect(m_hWindow, &pos);

			posX = pos.left;
			posY = pos.top;

			DEVMODE dmScreenSettings;
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = this->width;
			dmScreenSettings.dmPelsHeight = this->height;
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

			ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

			SetWindowLongPtr(m_hWindow, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
			MoveWindow(m_hWindow, 0, 0, width, height, TRUE);

			glViewport(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		}
		else
		{
			DEVMODE dmScreenSettings;
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			//dmScreenSettings.dmPelsWidth = this->width;
			//dmScreenSettings.dmPelsHeight = this->height;
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

			ChangeDisplaySettings(&dmScreenSettings, NULL);

			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = width;
			rect.bottom = height;
			if (maximized && this->width == GetSystemMetrics(SM_CXSCREEN))//unschöne lösung
			{
				SetWindowLongPtr(m_hWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE);
				AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE, FALSE);
			}
			else
			{
				SetWindowLongPtr(m_hWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
				AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
			}
			MoveWindow(m_hWindow, posX, posY, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		}
		//dwExStyle = WS_EX_APPWINDOW;
		//dwStyle = WS_POPUP;
		//ShowCursor(FALSE);
	}

	LRESULT Win32Window::OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (Message == WM_NCCREATE)
		{
			CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);

			SetWindowLongPtr(Handle, GWLP_USERDATA, reinterpret_cast<long>(pCreateStruct->lpCreateParams));
		}

		Win32Window* pWindow = reinterpret_cast<Win32Window*>(GetWindowLongPtr(Handle, GWLP_USERDATA));
		if (pWindow)
			pWindow->ProcessEvent(Message, wParam, lParam);

		return DefWindowProc(Handle, Message, wParam, lParam);
	}

	void Win32Window::ProcessEvent(UINT Message, WPARAM wParam, LPARAM lParam)
	{
		switch (Message)
		{
		case WM_CLOSE:
			///ostQuitMessage(0);
			close = true;
			break;
		case WM_SIZE:
			OnSize(LOWORD(lParam), HIWORD(lParam));
			if (!fullscreen)
			{
				width = LOWORD(lParam);
				height = HIWORD(lParam);
			}
			else
			{
				secWidth = LOWORD(lParam);
				secHeight = HIWORD(lParam);
			}
			break;
		case WM_GETMINMAXINFO:
			//if (((LPMINMAXINFO) lParam)->ptMaxPosition.x == 0 || ((LPMINMAXINFO) lParam)->ptMaxPosition.y == 0)
			maximized = true;
			break;
		case WM_KEYDOWN:
			Keyboard::setKeyState((KeyboardKeys)wParam, true);
			Keyboard::setKeyPressed((KeyboardKeys)wParam);
			break;
		case WM_KEYUP:
			Keyboard::setKeyState((KeyboardKeys)wParam, false);
			break;
		case WM_MOUSEMOVE:
			//komischer unterschied, ich mein wtf??? funktioniert das auf allen systemen?
			/*if (Mouse::isWrapped())
			{
				if (GetWindowRect(m_hWindow, &rect))
				{
					int width2 = (rect.right - rect.left);
					int height2 = (rect.bottom - rect.top);
					//Reporter::report(floatToString(LOWORD(lParam)) + " " + floatToString(HIWORD(lParam)));
					POINT p;
					if (GetCursorPos(&p))
					{
						Mouse::addPos(Vec2D(p.x - GetLocalCoordinates().left, p.y - GetLocalCoordinates().top), Vec2D(width2, height2));
						SetCursorPos(width2 / 2, height2 / 2);
						//Reporter::report(floatToString(p.x - GetLocalCoordinates().left) + " " + floatToString(p.y - GetLocalCoordinates().top));
					}
				}
			}
			else
			{
				Mouse::setPos(Vec2D(LOWORD(lParam), HIWORD(lParam)));
			}*/
			//Reporter::report(floatToString(p.x) + " " + floatToString(p.y));

			//Reporter::report(floatToString(LOWORD(lParam) + 8) + " " + floatToString(HIWORD(lParam) + 31));
			//if (!(LOWORD(lParam) + 8 == width / 2))
				//Mouse::setPos(Vec2D(LOWORD(lParam), 340));
			/*if (!(LOWORD(lParam) + 8 == width / 2 && HIWORD(lParam) + 31 == height / 2))
				Mouse::setPos(Vec2D(LOWORD(lParam), HIWORD(lParam)));*/
			break;
		case WM_LBUTTONDOWN:
			Mouse::setButtonPressed(ButtonLeft);
			Mouse::setButtonState(ButtonLeft, true);
			break;
		case WM_LBUTTONUP:
			Mouse::setButtonState(ButtonLeft, false);
			break;
		case WM_RBUTTONDOWN:
			Mouse::setButtonPressed(ButtonRight);
			Mouse::setButtonState(ButtonRight, true);
			break;
		case WM_RBUTTONUP:
			Mouse::setButtonState(ButtonRight, false);
			break;
		/*case WM_LBUTTONDBLCLK:
			mouse.clickedButtons[LEFTBUTTON] = true;
			break;
		case WM_RBUTTONDBLCLK:
			mouse.clickedButtons[RIGHTBUTTON] = true;
			break;*/
		case WM_SETCURSOR:
			if (LOWORD(lParam) && !cursorVisibility)
				SetCursor(NULL);
			else
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			break;
		}
	}

	void Win32Window::RegisterWindowClass()
	{
		WNDCLASS WindowClass;
		WindowClass.style = 0;
		WindowClass.lpfnWndProc = &Win32Window::OnEvent;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = GetModuleHandle(NULL);
		WindowClass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(icon));//IDI_ICON1
		WindowClass.hCursor = 0;
		WindowClass.hbrBackground = 0;
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = (LPCSTR)this->title;

		RegisterClass(&WindowClass);
	}

	void Win32Window::CreateContext()
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;

		m_hDeviceContext = GetDC(m_hWindow);

		int PixelFormat;

		PixelFormat = ChoosePixelFormat(m_hDeviceContext, &pfd);
		SetPixelFormat(m_hDeviceContext, PixelFormat, &pfd);
		m_hGLContext = wglCreateContext(m_hDeviceContext);

		int attribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 2, WGL_CONTEXT_MINOR_VERSION_ARB, 1, 0 };

		HGLRC fake = wglCreateContext(m_hDeviceContext);
		wglMakeCurrent(m_hDeviceContext, fake);

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

		m_hGLContext = wglCreateContextAttribsARB(m_hDeviceContext, 0, attribs);
		wglDeleteContext(fake);
		wglMakeCurrent(m_hDeviceContext, m_hGLContext);

		glewInit();
	}

	void Win32Window::OnSize(GLsizei width, GLsizei height)
	{
		realWidth = width;
		realHeight = height;
		Application::getActiveApplication()->setRendererResolution(width, height);
		Application::getActiveApplication()->onRender();
		/*if (fullscreen)
			glViewport(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		else
			glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (fullscreen)
			gluPerspective(fov, (float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN), 0.001, 1000);
		else
			gluPerspective(fov, (float)width / (float)height, 0.001, 1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//glShadeModel(GL_SMOOTH);
		//glClearDepth(1.0f);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
		//glMatrixMode(GL_MODELVIEW);
		if (render != NULL)
			render();
		SwapBuffer();*/
	}

	int Win32Window::getWidth()
	{
		return width;
	}

	int Win32Window::getHeight()
	{
		return height;
	}

	void Win32Window::SwapBuffer()
	{
		SwapBuffers(m_hDeviceContext);
	}

	void Win32Window::changeCursor(bool visibility)
	{
		cursorVisibility = visibility;
	}

	void Win32Window::changeWrap(bool wrap)
	{
		wrapCursor = wrap;
	}

	RECT Win32Window::GetLocalCoordinates() const
	{
		RECT Rect;
		GetWindowRect(m_hWindow, &Rect);
		MapWindowPoints(HWND_DESKTOP, GetParent(m_hWindow), (LPPOINT)&Rect, 2);
		return Rect;
	}

	void Win32Window::updateWindow()
	{
		if (Mouse::isWrapped())
		{
			RECT rect;
			if (GetWindowRect(m_hWindow, &rect))
			{
				int width2 = (rect.right - rect.left);
				int height2 = (rect.bottom - rect.top);
				//Reporter::report(floatToString(LOWORD(lParam)) + " " + floatToString(HIWORD(lParam)));
				POINT p;
				if (GetCursorPos(&p))
				{
					Mouse::addPos(Vec2D(p.x - GetLocalCoordinates().left, p.y - GetLocalCoordinates().top), Vec2D(width2, height2));
					SetCursorPos(GetLocalCoordinates().left + width2 / 2, GetLocalCoordinates().top + height2 / 2);
					Reporter::report(floatToString(p.x - GetLocalCoordinates().left) + " " + floatToString(p.y - GetLocalCoordinates().top));
					Reporter::report(floatToString(width2 / 2) + " " + floatToString(height2 / 2));
				}
			}
		}
		else
		{
			POINT p;
			if (GetCursorPos(&p))
				Mouse::setPos(Vec2D(p.x, p.y));
		}
		/*if (GetWindowRect(m_hWindow, &rect))
		{
			count++;
			int width2 = rect.right - rect.left;
			int height2 = rect.bottom - rect.top;
			//Reporter::report(floatToString(width2 / 2) + " " + floatToString(height2 / 2));
			if (wrapCursor)
				SetCursorPos(width2 / 2, height2 / 2);
			return count;

			/*POINT p;
			if (GetCursorPos(&p))
			{

				Reporter::report(floatToString(p.x - GetLocalCoordinates().left) + " " + floatToString(p.y - GetLocalCoordinates().top));
				//cursor position now in p.x and p.y
				if (ScreenToClient(m_hWindow, &p))
				{
					//p.x and p.y are now relative to hwnd's client area
				}
			}*/

			//Reporter::report(floatToString(p.x) + " " + floatToString(p.y));
		//}
		

		//Reporter::report(floatToString(width / 2) + " " + floatToString(height / 2));
	}
}