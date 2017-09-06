#pragma once
#include "Application.h"
#include "glm.hpp"
#include "FlyCamera.h"

class Mesh;


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
	unsigned int m_programID;
	Camera * m_camera = new FlyCamera();
	unsigned int m_row, m_column;
	Mesh * m_mesh;
};