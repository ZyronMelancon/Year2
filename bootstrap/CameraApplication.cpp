#define GLM_FORCE_SWIZZLE
#include "CameraApplication.h"
#include "Gizmos.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>


CameraApplication::CameraApplication(): m_camera(nullptr)
{
	
}


CameraApplication::~CameraApplication()
{
}

void CameraApplication::startup()
{
	Gizmos::create();
	m_camera = new FlyCamera();
}


void CameraApplication::draw()
{
	glm::mat4 s1 = glm::mat4(1);
	glm::vec4 center = glm::vec4(0, 0, 0, 1);
	glm::vec4 color = glm::vec4(0, 0, 0, 0);
	Gizmos::clear();
	Gizmos::addSphere(s1[3], 1, 20, 20, color);
	Gizmos::addTransform(s1, 4);
	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; i++)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}
	Gizmos::draw(m_camera->getProjectionView());
}

void CameraApplication::update(float)
{
}

void CameraApplication::shutdown()
{
}
