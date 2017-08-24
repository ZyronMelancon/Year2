#pragma once
#include "Application.h"
#include "FlyCamera.h"

class CameraApplication :
	public Application
{
public:
	CameraApplication();
	virtual ~CameraApplication();

	void startup() override;
	void draw() override;
	void update(float) override;
	void shutdown() override;

private:
	Camera * m_camera;
};