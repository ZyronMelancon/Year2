#pragma once
#include "glm.hpp"

enum Axis
{
	XAXIS,
	YAXIS,
	ZAXIS
};

class Transform
{
public:
	Transform();
	~Transform();
	glm::mat4 m_world;
	glm::mat4 m_local;
	glm::mat3 m_rotation;
	glm::vec3 m_position;

	void rotate(float radians, Axis rotationAxis);
};

