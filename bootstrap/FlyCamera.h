#pragma once
#include "Camera.h"

class FlyCamera :
	public Camera
{
public:
	FlyCamera();
	virtual ~FlyCamera();
	void update(float deltatime) override;
};