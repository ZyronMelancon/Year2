#pragma once
#include "Application.h"
#include "glm.hpp"
#include "FlyCamera.h"

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
};

class RenderApplication :
	public Application
{
private:

public:
	RenderApplication();
	virtual ~RenderApplication();
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	unsigned int indexCount;
	void generateGrid(unsigned int rows, unsigned int cols);
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_programID;
	Camera * cam = new FlyCamera();
	unsigned int m_row, m_column;
};