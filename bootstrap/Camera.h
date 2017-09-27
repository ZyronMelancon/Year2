#pragma once
#include <glm.hpp>
class Camera
{
public:
	Camera();
	virtual ~Camera();
	virtual void update(float deltatime) = 0;
	void setPerspective(float fov,float aspect,float near,float far);
	void setLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
	void setPosition(glm::vec3 pos);
	glm::mat4 getWorldTransform();
	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::mat4 getProjectionView();
	void setRotationX(double);
	glm::vec3 getPosition();
private:
	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_projectionViewTransform;

	void updateProjectionViewTransform();
};
