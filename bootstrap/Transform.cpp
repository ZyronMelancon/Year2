#include "Transform.h"
#include <gtx/rotate_vector.inl>


Transform::Transform()
{
}


Transform::~Transform()
{
}

void Transform::rotate(float radians, Axis rotationAxis)
{
	glm::vec3 xaxis = glm::vec3(1, 0, 0);
	glm::vec3 yaxis = glm::vec3(0, 1, 0);
	glm::vec3 zaxis = glm::vec3(0, 0, 1);
	switch (rotationAxis)
	{
	case ZAXIS:
			xaxis = glm::vec3(glm::cos(radians), -glm::sin(radians), 0);
			yaxis = glm::vec3(glm::sin(radians), glm::cos(radians), 0);
			zaxis = glm::vec3(0, 0, 1);
			break;
	case YAXIS:
			xaxis = glm::vec3(glm::cos(radians), 0, glm::sin(radians));
			yaxis = glm::vec3(0, 1, 0);
			zaxis = glm::vec3(-glm::sin(radians), 0, glm::cos(radians));
			break;
	case XAXIS:
			xaxis = glm::vec3(1, 0, 0);
			yaxis = glm::vec3(0, glm::cos(radians), -glm::sin(radians));
			zaxis = glm::vec3(0, glm::sin(radians), glm::cos(radians));
			break;
	default:
		break;
	}

	m_rotation = glm::mat3(xaxis, yaxis, zaxis);

	//glm::mat4 expected = glm::rotate(radians, rotationAxis);
}
