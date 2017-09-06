#include "RenderApplication.h"
#include <vector>
#include <gtc/constants.inl>

void main()
{
	auto app = new RenderApplication();
	app->run("Rendering Geometry",800,800,false);
}