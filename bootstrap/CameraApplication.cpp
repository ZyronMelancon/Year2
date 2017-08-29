#define GLM_FORCE_SWIZZLE
#include "CameraApplication.h"
#include "Gizmos.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <cstdio>


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
	m_camera->setPosition(glm::vec3(3, 2, 10));
	m_camera->setPerspective(1, 1, 0, 100);
}

glm::vec3 earthpos = glm::vec3(0, 0, 0);
void CameraApplication::draw()
{
	glm::mat4 s1 = glm::mat4(1);
	glm::vec4 center = glm::vec4(0, 0, 0, 1);
	glm::vec4 color = glm::vec4(0, 0, 0, 0);
	Gizmos::clear();
	Gizmos::addSphere(s1[3], 1, 20, 20, color);
	Gizmos::addTransform(s1, 4);
	Gizmos::addSphere(earthpos, 0.3, 10, 10, color);
	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; i++)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}
	Gizmos::draw(m_camera->getProjectionView());
}

float totalTime = 0;
static double c_mx, c_my; //Current mouse pos
static double p_mx, p_my; //Previous mouse pos
static double d_mx, d_my; //Delta mouse
void CameraApplication::update(float deltaTime)
{
	totalTime += deltaTime;
	earthpos.x = glm::cos(totalTime) * 5;
	earthpos.z = glm::sin(totalTime) * 5;

	bool pressed = false;
	glfwGetCursorPos(m_window, &c_mx, &c_my);
	d_mx = c_mx - p_mx;
	d_my = c_my - p_my;
	p_mx = c_mx;
	p_my = c_my;
	if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		pressed = true;
		glm::vec2 mousevector = glm::vec2(c_mx, c_my);
		printf("Mouse speed: %f, %f\n", d_mx, d_my);
		
		m_camera->setRotationX(d_mx / 800);
	}
	if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x + m_camera->getWorldTransform()[0].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z - m_camera->getWorldTransform()[2].z * deltaTime * 6);
		m_camera->setPosition(move);
		printf("W pressed!\n");
	}
	if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x - m_camera->getWorldTransform()[0].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z + m_camera->getWorldTransform()[2].z * deltaTime * 6);
		m_camera->setPosition(move);
		printf("S pressed!\n");
	}
	if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x - m_camera->getWorldTransform()[2].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z - m_camera->getWorldTransform()[0].z * deltaTime * 6);
		m_camera->setPosition(move);
		printf("A pressed!\n");
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x + m_camera->getWorldTransform()[2].z * deltaTime * 6, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z + m_camera->getWorldTransform()[0].z * deltaTime * 6);
		m_camera->setPosition(move);
		printf("D pressed!\n");
	}
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x, m_camera->getWorldTransform()[3].y + 6 * deltaTime, m_camera->getWorldTransform()[3].z);
		m_camera->setPosition(move);
		printf("Space pressed!\n");
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		glm::vec3 move = glm::vec3(m_camera->getWorldTransform()[3].x, m_camera->getWorldTransform()[3].y - 6 * deltaTime, m_camera->getWorldTransform()[3].z);
		m_camera->setPosition(move);
		printf("LCtrl pressed!\n");
	}
	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_camera->setLookAt(glm::vec3(m_camera->getWorldTransform()[3].x, m_camera->getWorldTransform()[3].y, m_camera->getWorldTransform()[3].z), glm::vec3(0, 0, 0),  glm::vec3(0, 1, 0));
		printf("Q pressed!\n");
	}
}

void CameraApplication::shutdown()
{
}
