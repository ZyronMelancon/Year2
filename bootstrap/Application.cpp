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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
}