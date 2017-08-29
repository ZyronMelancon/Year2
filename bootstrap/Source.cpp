#include "IntroApp.h"
#include "CameraApplication.h"


int main()
{
	Application* app = new CameraApplication();
	app->run("Intro",800,800,false);
	delete app;
}
