#pragma once
#include <Application.h>
#include <Camera.h>

class Mesh;
class Shader;

class TextureApplication :
	public Application
{
public:
	TextureApplication();
	virtual ~TextureApplication();
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	Mesh * m_mesh;
	Shader * m_shader;
	Camera * m_camera;
};

