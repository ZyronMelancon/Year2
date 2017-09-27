#include "Camera.h"
#include <gtc/matrix_transform.inl>


Camera::Camera() : m_worldTransform(glm::mat4(1)), m_viewTransform(glm::mat4(1)), m_projectionTransform(glm::mat4(1)), m_projectionViewTransform(glm::mat4(1))
{
}


Camera::~Camera()
{
}

void Camera::setPerspective(float fov, float aspect, float near, float far)
{
	glm::mat4 expected = glm::perspective(fov, aspect, near, far);

	//glm::mat4 view = glm::mat4(0);

	//view[0].x = 1 / tan(fov / 2);
	//view[1].y = 1 / (aspect / tan(fov / 2));
	//view[2].w = -1;
	//view[2].z = (far + near) / (far - near);
	//view[3].z = (2 * far*near) / (far - near);

	m_projectionTransform = expected;
}

void Camera::setLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	glm::vec3 z = normalize(eye - center);
	glm::vec3 x = normalize(cross(up, z));
	glm::vec3 y = cross(z, x);
	glm::mat4 view = glm::mat4(
		x[0], y[0], z[0], 0,
		x[1], y[1], z[1], 0,
		x[2], y[2], z[2], 0,
		0, 0, 0, 1);
	glm::mat4 translate = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(-eye.x, -eye.y, -eye.z, 1));
	view = view * translate;
	m_viewTransform = view;
	m_worldTransform = glm::inverse(view);
	m_projectionViewTransform = m_projectionTransform * view;
}

void Camera::setPosition(glm::vec3 pos)
{
	m_worldTransform[3] = glm::vec4(pos, 1);
	m_viewTransform = glm::inverse(m_worldTransform);
}

glm::mat4 Camera::getWorldTransform()
{
	return m_worldTransform;
}

glm::mat4 Camera::getView()
{
	return m_viewTransform;
}

glm::mat4 Camera::getProjection()
{
	return m_projectionTransform;
}

glm::mat4 Camera::getProjectionView()
{
	return m_projectionTransform * m_viewTransform;
}

void Camera::setRotationX(double rads)
{
	glm::mat4 rotateX =
		glm::mat4(
		(float)glm::cos(rads), 0.f, (float)glm::sin(rads), 0,
			0.f, 1.f, 0.f, 0,
			(float)-glm::sin(rads), 0.f, (float)glm::cos(rads), 0,
			0.f, 0.f, 0.f, 1.f);
	m_worldTransform = m_worldTransform * rotateX;
	m_viewTransform = glm::inverse(m_worldTransform);
}

glm::vec3 Camera::getPosition()
{
	return glm::vec3(getWorldTransform()[3][0], getWorldTransform()[3][1], getWorldTransform()[3][2]);
}