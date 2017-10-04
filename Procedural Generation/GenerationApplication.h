#pragma once
#include <Application.h>
#include <FlyCamera.h>

class Mesh;
class Shader;

class GenerationApplication :
	public Application
{
public:
	GenerationApplication();
	virtual ~GenerationApplication();
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	Camera * m_cam;
	Mesh * m_plane;
	Shader * m_shader;
};