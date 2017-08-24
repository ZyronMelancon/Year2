#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::setPerspective()
{

}

void Camera::setLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	glm::vec3 forw = eye - center;
	glm::vec3 z = glm::normalize(forw);
	glm::vec3 side = glm::cross(forw, up);
	glm::vec3 x = glm::normalize(side);
	glm::vec3 u = glm::cross(forw, x);
	glm::vec3 y = glm::normalize(u);

	glm::mat4 V = glm::mat4(x.x, y.x, z.x, 0, x.y, y.y, z.y, 0, x.z, y.z, z.z, 0, 0, 0, 0, 1);
	glm::mat4 T = glm::mat4(1, 0, 0, -eye.x, 0, 1, 0, -eye.y, 0, 0, 1, -eye.z, 0, 0, 0, 1);

	viewTransform = V * T;
	worldTransform = -viewTransform;
}

void Camera::setPosition(glm::vec3 pos)
{
	viewTransform[0][4] = pos.x;
	viewTransform[1][4] = pos.y;
	viewTransform[2][4] = pos.z;
	worldTransform = -viewTransform;
}

glm::mat4 Camera::getWorldTransform()
{
	return worldTransform;
}

glm::mat4 Camera::getView()
{
	return viewTransform;
}