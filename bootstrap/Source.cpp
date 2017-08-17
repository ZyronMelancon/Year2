#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glm.hpp>
#include <fwd.hpp>
#include <ext.hpp>
#include "Gizmos.h"


int main()
{
	glm::vec3 v = glm::vec3(1, 1, 1);

	if(!glfwInit())
	{
		printf("No GLFW\n");
		return -1;
	}

	GLFWwindow * window = glfwCreateWindow(800, 600, "GLFW", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
	}
	assert(window != nullptr);

	glfwMakeContextCurrent(window);


	if(ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	glm::vec4 clearcolor = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);

	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);
	glm::mat4 view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 4 / 3.f, 0.1f, 1000.f);

	glEnable(GL_DEPTH_TEST);

	while(true)
	{
		Gizmos::create();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if(glfwWindowShouldClose(window))
			break;
		if(glfwGetKey(window, GLFW_KEY_ESCAPE))
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


		for(int i = 0; i < 21; i++)
		{
			Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), i == 10 ? white : black);
			Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), i == 10 ? white : black);
		}
		Gizmos::addSphere(glm::vec3(0, 0, 0), 5.f, 25, 25, glm::vec4(.25f, .25f, .25f, 1.f));
		Gizmos::draw(projection * view);


		glClearColor(clearcolor.r, clearcolor.g, clearcolor.b, clearcolor.a);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	printf("GLFW found\n");
	return 0;
}