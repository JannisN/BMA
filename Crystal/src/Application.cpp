#include "Application.h"

namespace Crystal
{
	Application* Application::application;

	Application::Application()
	{
		setActiveApplication(this);
		renderer = new Renderer();
	}

	Application::~Application()
	{

	}

	void Application::setRendererResolution(int width, int height)
	{
		renderer->setResolution(width, height);
	}

	void Application::run()
	{
		if (glewInit() != GLEW_OK)
		{
			Reporter::errorReport("glewInit failed");
		}
		
		Reporter::report("test");

		renderScene = new RenderScene();
		renderScene->init(renderer);

		window->setFullscreen(window->isFullscreen());

		running = true;

		while (running)//vlt als thread um aussetzer zu verhindern
		{

			/*window->setCurrent();
			glfwPollEvents();
			glfwMakeContextCurrent(NULL);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));*/

			timestamp.updateTime();

			window->wrapCursor();
			//Reporter::report(window->wrapCursor() % 144);
			window->updateCursor();
			Mouse::update(window->getWidth(), window->getHeight(), timestamp);


			renderer->update(timestamp);
			renderScene->update(timestamp, renderer->getCamera());
			onRender();

			Keyboard::update();

			//window->wrapCursor();

			running = !window->windowShouldClose();

			window->update();
		}
	}
	
	void Application::onRender()
	{
		if (running)
		{
			renderScene->render(renderer);
			window->swapBuffers();
		}
	}

	void Application::quit()
	{
		running = false;
	}

	Window* Application::getWindow()
	{
		return window;
	}


	void Application::createWindow(int width, int height, char* title, bool fullscreen)
	{
		renderer = new Renderer(width, height);
		window = new Window(width, height, title, renderer);
		window->setFullscreen(fullscreen);
	}

	void Application::createWindow(int width, int height, char* title)
	{
		renderer = new Renderer(width, height);
		window = new Window(width, height, title, renderer);
	}

	void Application::createWindow(bool fullscreen)
	{
		renderer = new Renderer();
		window = new Window(fullscreen, renderer);
	}

	void Application::createWindow(char* title)
	{
		renderer = new Renderer(720, 480);
		window = new Window(720, 480, title, renderer);
	}

	void Application::createWindow(int width, int height)
	{
		renderer = new Renderer(width, height);
		window = new Window(width, height, renderer);
	}

	void Application::createWindow()
	{
		renderer = new Renderer();
		window = new Window(renderer);
		//renderer = new Renderer(window->getWidth(), window->getHeight());
	}

	void Application::setActiveApplication(Application* app)
	{
		application = app;
	}

	Application* Application::getActiveApplication()
	{
		return application;
	}
}