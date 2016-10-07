#include "Application.h"

using namespace Crystal;

int main(void)
{
	Application app;

	app.createWindow();
	app.getWindow()->hideCursor(true);
	app.getWindow()->setFullscreen(false);
	app.getWindow()->setWrap(false);
	//app.getWindow()->setSize(3440, 1440);
	//Mouse::setWrap(true);
	app.run();

	return 0;
}