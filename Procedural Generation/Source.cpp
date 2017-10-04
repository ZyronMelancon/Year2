#include "GenerationApplication.h"

void main()
{
	Application * app = new GenerationApplication();
	app->run("Procedural Generation", 900, 650, false);
}
