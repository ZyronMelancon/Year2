#pragma once
#include "Application.h"
#include "FlyCamera.h"

class CamApp :
	public Application
{
public:
	CamApp();
	virtual ~CamApp();

	void startup() override;
	void draw() override;
	void update(float) override;
	void shutdown() override;

private:
	Camera * m_flyCam;
};

