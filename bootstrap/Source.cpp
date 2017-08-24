#include "IntroApp.h"
#include "CameraApplication.h"


int main()
{
	Application* app = new CameraApplication();
	app->run("Intro",800,600,false);
	delete app;
}
