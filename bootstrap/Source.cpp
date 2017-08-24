#include "IntroApp.h"


int main()
{
	Application* app = new IntroApp();
	app->run("Intro",800,600,false);
	delete app;
}