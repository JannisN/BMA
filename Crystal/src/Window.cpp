#include "Window.h"

namespace Crystal
{
	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Application::getActiveApplication()->getWindow()->setSize(width, height, !Application::getActiveApplication()->getWindow()->isFullscreen());
	}

	void window_refresh(GLFWwindow* window)
	{
		Application::getActiveApplication()->onRender();
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			Keyboard::setKeyState((KeyboardKeys)key, true);
			Keyboard::setKeyPressed((KeyboardKeys)key);
		}
		else if (action == GLFW_RELEASE)
		{
			Keyboard::setKeyState((KeyboardKeys)key, false);
		}
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		Reporter::report(floatToString(action) + " " + floatToString(button));

		if (action == GLFW_PRESS)
		{
			Mouse::setButtonState((MouseButtons)button, true);
			Mouse::setButtonPressed((MouseButtons)button);
		}
		else if (action == GLFW_RELEASE)
		{
			Mouse::setButtonState((MouseButtons)button, false);
		}
	}

	void cursor_position_callback(GLFWwindow* window, double x, double y)
	{
		if (Mouse::isWrapped())
		{
			Mouse::setPos(Vec2D(x, y));
			//Mouse::setPos(Mouse::getPos() - (Vec2D(x - Application::getActiveApplication()->getWindow()->getWidth() / 2, y - Application::getActiveApplication()->getWindow()->getHeight() / 2)));
		}
		else
		{
			Mouse::setPos(Vec2D(x, y));
		}
	}

	Window::Window(bool fullscreen, Renderer* renderer)
	{
#ifdef WINAPI
		this->renderer = renderer;

		window = new Win32Window(1200, 720, true, "Crystal Engine", GetModuleHandle(NULL));
		window->setFullscreen(true);

		this->height = height;
		this->width = width;

		//window->updateLoop();
#else
		if (!glfwInit())
		{
			Reporter::errorReport("Failed to initialize GLFW");
			glfwTerminate();
		}

		this->fullscreen = true;

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		window = glfwCreateWindow(mode->width, mode->height, "Crystal Engine", glfwGetPrimaryMonitor(), NULL);
		if (!window)
		{
			Reporter::errorReport("Failed to create Window");
			glfwTerminate();
		}

		this->renderer = renderer;

		this->height = mode->height;
		this->width = mode->width;
		this->oldHeight = 720;
		this->oldWidth = 1200;

		glfwMakeContextCurrent(window);
		glfwSetWindowSizeCallback(window, window_size_callback);
		glfwSetWindowRefreshCallback(window, window_refresh);
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		//glfwSetCursorPosCallback(window, cursor_position_callback);
#endif
	}

	Window::Window(int width, int height, char* title, Renderer* renderer)
	{
#ifdef WINAPI
		this->renderer = renderer;

		window = new Win32Window(width, height, true, title, GetModuleHandle(NULL));

		this->height = height;
		this->width = width;

		//window->updateLoop();
#else
		if (!glfwInit())
		{
			Reporter::errorReport("Failed to initialize GLFW");
			glfwTerminate();
		}

		this->fullscreen = false;

		window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!window)
		{
			Reporter::errorReport("Failed to create Window");
			glfwTerminate();
		}

		this->renderer = renderer;

		this->height = height;
		this->width = width;
		this->oldHeight = height;
		this->oldWidth = width;

		glfwMakeContextCurrent(window);
		glfwSetWindowSizeCallback(window, window_size_callback);
		glfwSetWindowRefreshCallback(window, window_refresh);
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		//glfwSetCursorPosCallback(window, cursor_position_callback);
#endif
	}

	Window::Window(int width, int height, Renderer* renderer) : Window(width, height, "Crystal Engine", renderer)
	{
	}

	Window::Window(Renderer* renderer) : Window(1200, 720, "Crystal Engine", renderer)
	{
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::setSize(int width, int height, bool changeOld)
	{
		this->width = width;
		this->height = height;

		if (changeOld)
		{
			this->oldWidth = width;
			this->oldHeight = height;
		}

		renderer->setResolution(width, height);
#ifdef WINAPI
		window->setWindowSize(width, height);
#else
		glfwSetWindowSize(window, width, height);
#endif
	}

	void Window::setSize(int width, int height)
	{
		this->width = width;
		this->height = height;
		this->oldWidth = width;
		this->oldHeight = height;

		renderer->setResolution(width, height);
#ifdef WINAPI
		window->setWindowSize(width, height);
#else
		glfwSetWindowSize(window, width, height);
#endif
	}

	void Window::setTitle(char* title)
	{
		this->title = title;
#ifdef WINAPI
		//TODO
#else
		glfwSetWindowTitle(window, title);
#endif
	}

	int Window::getHeight()
	{
#ifdef WINAPI
		return window->getHeight();
#else
		return height;
#endif
	}

	int Window::getWidth()
	{
#ifdef WINAPI
		return window->getWidth();
#else
		return width;
#endif
	}

	void Window::setFullscreen(bool option)
	{
		this->fullscreen = option;

#ifdef WINAPI
		window->setFullscreen(option);
#else
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		if (option)
		{
			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
			setSize(mode->width, mode->height, false);
		}
		else
		{
			Reporter::report(floatToString(width) + " " + floatToString(height) + " " + floatToString(oldWidth) + " " + floatToString(oldHeight));
			width = oldWidth;
			height = oldHeight;

			glfwSetWindowMonitor(window, 0, (mode->width - this->width) / 2, (mode->height - this->height) / 2, this->width, this->height, mode->refreshRate);

			setSize(this->width, this->height);
		}
#endif
	}

	bool Window::isFullscreen()
	{
		return this->fullscreen;
	}

	void Window::toggleFullscreen()
	{
		fullscreen = !fullscreen;

		setFullscreen(fullscreen);
	}

	void Window::hideCursor(bool option)
	{
#ifdef WINAPI
		window->changeCursor(!option);
#else
		if (option)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif
	}

	void Window::setResizable(bool option)
	{
#ifdef WINAPI
		//todo
#else
		glfwWindowHint(GLFW_RESIZABLE, option);	
#endif
	}

	void Window::enableVSync(bool option)
	{
		vsync = option;

#ifdef WINAPI
		//todo
#else
		glfwSwapInterval(vsync);
#endif
	}

	void Window::swapBuffers()
	{
#ifdef WINAPI
		window->SwapBuffer();
#else
		glfwSwapBuffers(window);
#endif
	}

	bool Window::windowShouldClose()
	{
#ifdef WINAPI
		return window->shouldClose();
#else
		return glfwWindowShouldClose(window);
#endif
	}

	void Window::setRenderer(Renderer* renderer)
	{
		this->renderer = renderer;
	}

	void Window::wrapCursor()
	{
#ifdef WINAPI
		window->updateWindow();
#endif
		/*if (Mouse::isWrapped())
		{
			glfwSetCursorPos(window, width / 2, height / 2);
		}*/
	}

	void Window::setWrap(bool option)
	{
#ifdef WINAPI
		window->changeWrap(option);
		Mouse::setWrap(option);
#else
		if (option)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif
	}

	void Window::updateCursor()
	{
#ifndef WINAPI
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);

		Mouse::setPos(Vec2D(x, y));
#endif
	}

	void Window::update()
	{
#ifdef WINAPI
		window->updateLoop();
#else
		glfwPollEvents();
#endif
	}

	void Window::setCurrent()
	{
#ifndef WINAPI
		glfwMakeContextCurrent(window);
#endif
	}
}