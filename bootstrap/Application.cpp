#include "Application.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Gizmos.h"


Application::Application()
{
}


Application::~Application()
{
}


void Application::run(const char * title, unsigned int width, unsigned int height, bool fullscreen)
{
	//Start???
	
	if (!glfwInit())
		return;



	//Set primary monitor to fullscreen if fullscreen is true
	GLFWmonitor * monitor = (fullscreen) ? glfwGetPrimaryMonitor() : nullptr;
	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);


	glfwMakeContextCurrent(m_window);
	//Load the opengl function pointers
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return;
	}

	startup();


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

	//Timing stuff
	double prevTime = glfwGetTime();
	double currTime = 0;
	double deltaTime = 0;

	//Main loop
	while (!m_gameover)
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;
		prevTime = currTime;
		glfwPollEvents();
		update(deltaTime);
		draw();
		glfwSwapBuffers(m_window);
		m_gameover = (glfwWindowShouldClose(m_window) == GLFW_TRUE);
	}
	glfwDestroyWindow(m_window);
	glfwTerminate();

	//if (m_window == nullptr)
	//{
	//	glfwTerminate();
	//}
	//assert(m_window != nullptr);

	//glfwMakeContextCurrent(m_window);



	//auto major = ogl_GetMajorVersion();
	//auto minor = ogl_GetMinorVersion();
	//printf("GL: %i.%i\n", major, minor);




	//glm::vec4 white(1);
	//glm::vec4 black(0, 0, 0, 1);
	//glm::mat4 view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	//glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 4 / 3.f, 0.1f, 1000.f);



	/*while (true)
	{
	Gizmos::create();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (glfwWindowShouldClose(window))
	break;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_F))
	{
	clearcolor.r = clearcolor.r * .9f;
	printf("clearcolor: %f, %f, %f\n", clearcolor.r, clearcolor.g, clearcolor.b);
	}
	if (glfwGetKey(window, GLFW_KEY_R) && clearcolor.r < 1)
	{
	clearcolor.r = clearcolor.r * 1.1f;
	printf("clearcolor: %f, %f, %f\n", clearcolor.r, clearcolor.g, clearcolor.b);
	}
	if (glfwGetKey(window, GLFW_KEY_G))
	{
	clearcolor.g = clearcolor.g * .9f;
	printf("clearcolor: %f, %f, %f\n", clearcolor.r, clearcolor.g, clearcolor.b);
	}
	if (glfwGetKey(window, GLFW_KEY_T) && clearcolor.g < 1)
	{
	clearcolor.g = clearcolor.g * 1.1f;
	printf("clearcolor: %f, %f, %f\n", clearcolor.r, clearcolor.g, clearcolor.b);
	}
	if (glfwGetKey(window, GLFW_KEY_Y) && clearcolor.b < 1)
	{
	clearcolor.b = clearcolor.b * 1.1f;
	printf("clearcolor: %f, %f, %f\n", clearcolor.r, clearcolor.g, clearcolor.b);
	}
	if (glfwGetKey(window, GLFW_KEY_H))
	{
	clearcolor.b = clearcolor.b * .9f;
	printf("clearcolor: %f, %f, %f\n", clearcolor.r, clearcolor.g, clearcolor.b);
	}

	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1));


	for (int i = 0; i < 21; i++)
	{
	Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), i == 10 ? white : black);
	Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}
	Gizmos::addSphere(glm::vec3(0, 0, 0), 5.f, 25, 25, glm::vec4(.25f, .25f, .25f, 1.f));
	Gizmos::draw(projection * view);


	glClearColor(clearcolor.r, clearcolor.g, clearcolor.b, clearcolor.a);
	glfwSwapBuffers(window);
	glfwPollEvents();
	}*/

	//glfwDestroyWindow(m_window);
	//glfwTerminate();
	//printf("GLFW found\n");
}
